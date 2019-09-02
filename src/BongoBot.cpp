#include "BongoBot.h"
#include "utils.h"

#define IP_MTU_SIZE 1536


BongoBot::BongoBot(){
	
	this->name = "BongoBot";
	this->oscAddress = "/BongoBot";
	this->receivePort = 1234;
	this->sendPort = 1234;
	this->ip = utils::getIpAddress();
	this->familyType = "percussion";
	this->specificProtocol = "</playBongo; velocity_i>";
	
}

void BongoBot::sendHandshake(){
	
	char buffer[IP_MTU_SIZE];
    osc::OutboundPacketStream p( buffer, IP_MTU_SIZE );
	UdpTransmitSocket socket(  IpEndpointName( "192.168.0.103", 1234 )  );

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

}
