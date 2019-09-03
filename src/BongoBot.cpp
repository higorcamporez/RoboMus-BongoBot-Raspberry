#include "BongoBot.h"
#define NTP_TIMESTAMP_DIFF   (2208988800) 
#define IP_MTU_SIZE 1536


BongoBot::BongoBot(){
	
	this->name = "BongoBot";
	this->oscAddress = "/BongoBot";
	this->receivePort = 1234;
	this->sendPort = 1234;
	this->ip = utils::getIpAddress();
	this->familyType = "percussion";
	this->specificProtocol = "</playBongo; velocity_i>";
	
	this->messages = new vector<RoboMusMessage*> ;
	
	std::thread t(&BongoBot::messageController, this);
	t.detach();
	
}

BongoBot::~BongoBot(){

}

void BongoBot::sendHandshake(){
	
	char buffer[IP_MTU_SIZE];
    osc::OutboundPacketStream p( buffer, IP_MTU_SIZE );
    //IpEndpointName( IpEndpointName::ANY_ADDRESS, PORT )
	UdpTransmitSocket socket(  IpEndpointName("192.168.0.103",1234)  );

    p.Clear();
   
    p << osc::BeginMessage( "/handshake/instrument" )
				<< this->name.c_str()
				<< this->oscAddress.c_str()
				<< this->ip.c_str()
				<< this->receivePort
				<< this->polyphony
				<< this->familyType.c_str()
				<< this->specificProtocol.c_str()
				<< osc::EndMessage;
	
	
    
				
    socket.Send( p.Data(), p.Size() );
    
    UdpListeningReceiveSocket s(
            IpEndpointName( IpEndpointName::ANY_ADDRESS, this->receivePort ),
            this );
            
    std::cout << "press ctrl-c to end\n";
	s.RunUntilSigInt();
}

void BongoBot::receiveHandshake(osc::ReceivedMessageArgumentStream args){
	const char *a1;
	const char *a2;
	const char *a3;
	osc::int32 a4;
	
	args >> a1 >> a2 >> a3 >> a4 >> osc::EndMessage;
	
	this->serverName = a1;
	this->serverOscAddress = a2;
	this->serverIpAddress = a3;
	this->sendPort = a4;
	
	std::cout << "received '/handshake' message with arguments: "
                    << a1 << " " << a2 << " " << a3 << " " << a4 << "\n";
}

void BongoBot::ProcessMessage( const osc::ReceivedMessage& m, 
				const IpEndpointName& remoteEndpoint ){
        (void) remoteEndpoint; // suppress unused parameter warning

        try{
            // example of parsing single messages. osc::OsckPacketListener
            // handles the bundle traversal.
            
            if( std::strcmp( m.AddressPattern(), (this->oscAddress+"/handshake").c_str() ) == 0 ){
                // example #1 -- argument stream interface
                osc::ReceivedMessageArgumentStream args = m.ArgumentStream();
                BongoBot::receiveHandshake(args);
                
                
            }else if( std::strcmp( m.AddressPattern(), "/test2" ) == 0 ){
                // example #2 -- argument iterator interface, supports
                // reflection for overloaded messages (eg you can call 
                // (*arg)->IsBool() to check if a bool was passed etc).
                osc::ReceivedMessage::const_iterator arg = m.ArgumentsBegin();
                bool a1 = (arg++)->AsBool();
                int a2 = (arg++)->AsInt32();
                float a3 = (arg++)->AsFloat();
                const char *a4 = (arg++)->AsString();
                if( arg != m.ArgumentsEnd() )
                    throw osc::ExcessArgumentException();
                
                std::cout << "received '/test2' message with arguments: "
                    << a1 << " " << a2 << " " << a3 << " " << a4 << "\n";
            }
        }catch( osc::Exception& e ){
            // any parsing errors such as unexpected argument types, or 
            // missing arguments get thrown as exceptions.
            std::cout << "error while parsing message: "
                << m.AddressPattern() << ": " << e.what() << "\n";
        }
    }

void BongoBot::insertMessage(RoboMusMessage *roboMusMessage){
	//std::cout<<"insertMessage"<<std::endl;

	for (std::vector<RoboMusMessage*>::iterator it = this->messages->begin() ; it != this->messages->end(); ++it){
		//std::cout << ' ' << (*it)->getTimetag();
		if((*it)->getTimetag() > roboMusMessage->getTimetag()){
			this->messages->insert(it, roboMusMessage);	
			return;	
		}
	}
	this->messages->push_back(roboMusMessage);
	
	
}

void BongoBot::ProcessBundle( const osc::ReceivedBundle& b,
			const IpEndpointName& remoteEndpoint )
{

	osc::ReceivedMessage m = osc::ReceivedMessage(*b.ElementsBegin());
	try{
		
		if( std::strcmp( m.AddressPattern(), (this->oscAddress+"/playBongo").c_str() ) == 0 ){
			// example #1 -- argument stream interface
			osc::ReceivedMessageArgumentStream args = m.ArgumentStream();
			osc::int64 a1;
			osc::int32 a2;
			
			args >> a1 >> a2 >> osc::EndMessage;
			
			unsigned long long t =  (b.TimeTag())>>32;
			t = t - NTP_TIMESTAMP_DIFF;
			unsigned long us =  ((unsigned long)(b.TimeTag()&0xffffffff));
			us = ((double)us/0xffffffff)*1000000;
			//long long time = t*1000000 + us;
			unsigned long long time = utils::convertNTPtoUTC(b.TimeTag());
			std::cout<<"/playBongo"<<" id = "<<a1<<"b.TimeTag() = "<<t<<" us="<<time<<std::endl;
			
			Action *a = new PlayBongo();
			RoboMusMessage *rmm = new RoboMusMessage(
											b.TimeTag(),
											a1,
											a);
			this->insertMessage(rmm);

		}
	}catch( osc::Exception& e ){
		// any parsing errors such as unexpected argument types, or 
		// missing arguments get thrown as exceptions.
		std::cout << "error while parsing message: "
			<< m.AddressPattern() << ": " << e.what() << "\n";
	}
}

void BongoBot::messageController(){
	cout<<"BongoBot::messageController()"<<endl;
	while(true){
		if(this->messages->size() > 0){
			RoboMusMessage* rmm = (*this->messages->begin());
			//cout<<rmm->getTimetag()<<" "<<utils::getCurrentTimeMillis()<<endl;
			if(rmm->getTimetag() < utils::getCurrentTimeMillis()){
				rmm->play();
				std::cout<<"play()"<<std::endl;
			}
		}
	}
}
