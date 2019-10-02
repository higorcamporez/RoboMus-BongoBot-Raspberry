#include "RoboMusMessage.h"
#include "utils.h"
#include <iostream>
#include <wiringPi.h>
#include "osc/OscOutboundPacketStream.h"
//#include "osc/OscReceivedElements.h"
#include "ip/UdpSocket.h"
//#include "ip/IpEndpointName.h"
#include <thread> 
#include <mutex>
#include <unistd.h> 

class PlayBongo : public Action{
	private:
		
		osc::int64 messageId;
		string serverOscAddress;
		string instrumentOscAddres;
		string serverIpAddress;
		int serverPort;
		thread *threadObj;
		thread *threadObjWaiting;
		unsigned long long startTime;
		
		int sock;
		struct sockaddr_in serverAddr; 
		 
	public:
	
		PlayBongo(
					long messageId,
					string instrumentOscAddres,
					string serverOscAddress,
					int serverPort,
					string serverIpAddress
					
				);
				
		~PlayBongo();
		void playInstrument();
		void watingBeat();
		void sendDelay();
		void play();
};
