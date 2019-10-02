#include "BongoBot.h"
#define NTP_TIMESTAMP_DIFF   (2208988800) 
#define IP_MTU_SIZE 1500
#define DEBUG 

BongoBot::BongoBot(){

	string mac = utils::getMacAddress();
	cout<<"mac = "<<mac;
	if("b8:27:eb:fd:6b:df" == mac){
		this->name = "BongoBot";
		this->oscAddress = "/BongoBot";
	}else{
		this->name = "BongoBot2";
		this->oscAddress = "/BongoBot2";
	}
	
	this->receivePort = 1234;
	this->sendPort = 1234;
	this->ip = utils::getIpAddress();
	this->familyType = "percussion";
	this->specificProtocol = "</playBongo;velocity_i>";
	
	this->nextRoboMusMessage = NULL;
	this->s = new UdpListeningReceiveSocket(
            IpEndpointName( IpEndpointName::ANY_ADDRESS, this->receivePort ),
            this );
    
            
	this->messages = new list<RoboMusMessage*> ;
	this->countLostMsgs = 0;
	this->countMsgsArraivedLate = 0;
	this->lastMsgId = 0;
	this->countLostMsgsNet = 0;
	
	
	this->outFileError.open("log_error.txt");
	this->outFileLog.open("log.txt");
	
	this->outFileLog<<"OscAddress, MessageId, MessageTime, CurrentTime"<<endl;
	
	std::thread t(&BongoBot::messageController, this);
	t.detach();
	
	utils::stopServiceNTP();
	std::thread t2(&BongoBot::timeSynchronizer, this);
	t2.detach();
	
	std::thread t3(&BongoBot::sendHandshake, this);
	t3.detach();
	
	
}

BongoBot::~BongoBot(){
	delete s;
	delete nextRoboMusMessage;
	
	for (std::list<RoboMusMessage*>::iterator it = this->messages->begin() ; it != this->messages->end(); ++it){
		
		delete (*it);
			
	}
	
	delete messages;
	
	delete transmitSocket;
	delete s;
}

void BongoBot::sendHandshake(){
	
	char buffer[IP_MTU_SIZE];
    osc::OutboundPacketStream p( buffer, IP_MTU_SIZE );

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
    
				

    ///////////////////////////
    // meu broadcast
    int sock;                         /* Socket */
    struct sockaddr_in broadcastAddr; /* Broadcast address */
    char broadcastIP[16];             /* IP broadcast address */
    unsigned short broadcastPort;     /* Server port */
    int broadcastPermission;          /* Socket opt to set permission to broadcast */
    //unsigned int sendStringLen;       /* Length of string to broadcast */
    
    strcpy(broadcastIP, "255.255.255.255");            /* First arg:  broadcast IP address */ 
    broadcastPort = 1234;    /* Second arg:  broadcast port */
    
    /* Create socket for sending/receiving datagrams */
    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        printf("socket() failed");

    /* Set socket to allow broadcast */
    broadcastPermission = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (void *) &broadcastPermission, 
          sizeof(broadcastPermission)) < 0)
        printf("setsockopt() failed");

    /* Construct local address structure */
    memset(&broadcastAddr, 0, sizeof(broadcastAddr));   /* Zero out structure */
    broadcastAddr.sin_family = AF_INET;                 /* Internet address family */
    broadcastAddr.sin_addr.s_addr = inet_addr(broadcastIP);/* Broadcast IP address */
    broadcastAddr.sin_port = htons(broadcastPort);         /* Broadcast port */

    //sendStringLen = strlen(sendString);  /* Find length of sendString */

	/* Broadcast sendString in datagram to clients every 3 seconds*/
    if (sendto(sock, p.Data(),  p.Size() , 0, (struct sockaddr *) 
	   &broadcastAddr, sizeof(broadcastAddr)) != (int)p.Size())
		printf("sendto() sent a different number of bytes than expected");

    close(sock);
    /////////////////////////
    /*
    UdpListeningReceiveSocket s(
            IpEndpointName( IpEndpointName::ANY_ADDRESS, this->receivePort ),
            this );
            
    std::cout << "press ctrl-c to end\n";
	s.RunUntilSigInt();
	std::cout << "pass\n";
	* */
	
	s->RunUntilSigInt();
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
	
	this->transmitSocket = new UdpTransmitSocket(
				IpEndpointName( this->serverIpAddress.c_str(), this->sendPort )
			);
	
	std::cout << "received '/handshake' message with arguments: "
                    << a1 << " " << a2 << " " << a3 << " " << a4 << "\n";
    this->syncTime();
  
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
	
	for (std::list<RoboMusMessage*>::iterator it = this->messages->begin() ; it != this->messages->end(); ++it){
		//std::cout << ' ' << (*it)->getTimetag();
		//std::cout<<"insertMessage"<<std::endl;
		if((*it)->getTimetag() > roboMusMessage->getTimetag()){
			
			this->messages->insert(it, roboMusMessage);
			//this->mtx.unlock();	
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
		unsigned long long time = utils::convertNTPtoUTC(b.TimeTag());
		if(time < utils::getCurrentTimeMicros()){
			
			osc::ReceivedMessageArgumentStream args = m.ArgumentStream();
			osc::int64 a1;
			osc::int32 a2;
			
			args >> a1 >> a2 >> osc::EndMessage;
			#ifdef DEBUG
			cout<<"The message "<<m.AddressPattern()<<" "<<a1<<" arrived too late"<<endl;
			#endif
			this->outFileError<<"The message "<<m.AddressPattern()<<" "<<a1<<" arrived too late"<<endl;
			this->outFileLog<<m.AddressPattern()<<","<<a1<<","<<time<<","<<utils::getCurrentTimeMicros()<<std::endl;
			
			this->countMsgsArraivedLate++;
			
			while(this->lastMsgId + 1 < a1){
				#ifdef DEBUG
				cout<<"Lost the Menssage "<<this->lastMsgId + 1<<endl;
				#endif
				this->outFileError<<"Lost the Menssage "<<this->lastMsgId + 1<<endl;
				this->lastMsgId++;
				this->countLostMsgsNet++;
			}
			this->lastMsgId = a1;
			
			
			
		}else if( std::strcmp( m.AddressPattern(), (this->oscAddress+"/playBongo").c_str() ) == 0 ){
			// example #1 -- argument stream interface
			osc::ReceivedMessageArgumentStream args = m.ArgumentStream();
			osc::int64 a1;
			osc::int32 a2;
			
			args >> a1 >> a2 >> osc::EndMessage;
			
			unsigned long long time = utils::convertNTPtoUTC(b.TimeTag());
			
			#ifdef DEBUG
			std::cout<<"/playBongo"<<" id = "<<a1<<" TimeTag="<<time<<" "<<utils::getCurrentTimeMicros()<<std::endl;
			#endif
			
			this->outFileLog<<m.AddressPattern()<<", "<<a1<<","<<time<<","<<utils::getCurrentTimeMicros()<<std::endl;
			
			Action *a = new PlayBongo(
										a1,
										this->oscAddress,
										this->serverOscAddress,
										this->sendPort,
										this->serverIpAddress
									);
			
			RoboMusMessage *rmm = new RoboMusMessage(
											time,
											a1,
											a);
			this->insertMessage(rmm);
			
			
			while(this->lastMsgId + 1 < a1){
				cout<<"Lost the Menssage "<<this->lastMsgId + 1<<endl;
				this->outFileError<<"Lost the Menssage "<<this->lastMsgId + 1<<endl;
				this->lastMsgId++;
				this->countLostMsgsNet++;
			}
			this->lastMsgId = a1;

			
		}else if( std::strcmp( m.AddressPattern(), (this->oscAddress+"/show").c_str() ) == 0 ){
			cout<<"Number of lost messagens because of delay in main loop "<<countLostMsgs<<endl;
			cout<<"Number of that arrived too late "<<countMsgsArraivedLate<<endl;
			cout<<"Number of lost messages "<<countLostMsgsNet<<endl;
			
			this->mtx.lock();
			this->outFileError<<"Number of lost messagens because of delay in main loop "<<countLostMsgs<<endl;
			this->outFileError<<"Number of that arrived too late "<<countMsgsArraivedLate<<endl;
			this->outFileError<<"Number of lost messages "<<countLostMsgsNet<<endl;
			this->mtx.unlock();
			
			
			this->countLostMsgs = 0;
			this->lastMsgId = 0;
			this->countMsgsArraivedLate = 0;
			this->countLostMsgsNet = 0;
			
			
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
	long long diff;
	while(true){
		
		if(this->nextRoboMusMessage != NULL){
			//this->mtx.lock();
			//RoboMusMessage* rmm = (*this->messages->begin());
			//cout<<rmm->getTimetag()<<" "<<utils::getCurrentTimeMicros()<<endl;
			//std::cout<<"messageController"<<std::endl;
			//this->mtx.unlock();
			diff = utils::getCurrentTimeMicros() - nextRoboMusMessage->getTimetag();
			
			if(diff >=0 && diff <= 1000){ //1000 us
				
				nextRoboMusMessage->play();
				#ifdef DEBUG
				cout<<"Play() id = "<<nextRoboMusMessage->getMessageId()<<" "<<diff<<endl;
				#endif
				
				this->mtx.lock();
				this->messages->erase(this->messages->begin());
				delete nextRoboMusMessage;
				this->mtx.unlock();
				this->nextRoboMusMessage = this->getNextMessage();
				
				//this->outFileLog<<"Play() "<<nextRoboMusMessage->getMessageId()<<" "<<diff<<endl;
				
				
			}else if(diff > 1000){
				this->mtx.lock();
				
				#ifdef DEBUG
				cout<<"M: "<<nextRoboMusMessage->getMessageId()<<" deleted "<<diff<<endl;
				#endif
				
				this->outFileError<<"M: "<<nextRoboMusMessage->getMessageId()<<" deleted "<<diff<<endl;
				
				delete nextRoboMusMessage;
				
				
				this->messages->erase(this->messages->begin());
				this->nextRoboMusMessage = this->getNextMessage();
				
				this->countLostMsgs++;
				this->mtx.unlock();
			}else if(diff < -10000){
				this->mtx.lock();
				this->nextRoboMusMessage = this->getNextMessage();
				this->mtx.unlock();
			}
			
		}else{
			this->mtx.lock();
			this->nextRoboMusMessage = this->getNextMessage();
			this->mtx.unlock();
		}
		
	}
}
RoboMusMessage* BongoBot::getNextMessage(){
	
	if(this->messages->size() > 0){
		return *(this->messages->begin());
	}else{
		return NULL;
	}
}

void BongoBot::syncTime(){
	utils::NTPSynchronizer(this->getServerIpAddress());
}

void BongoBot::timeSynchronizer(){
	long long t = 0;
	while(true){
		
		while(utils::getCurrentTimeMillis() - t > 20000){
			if(this->getServerIpAddress().length() > 0){
				this->syncTime();
				t = utils::getCurrentTimeMillis();
				//cout<<"t="<<t<<endl;
			}
		}
	}
}

void BongoBot::closeFiles(){
	
	this->mtx.lock();
	this->outFileError.close();
	this->outFileLog.close();
	this->mtx.unlock();

}
