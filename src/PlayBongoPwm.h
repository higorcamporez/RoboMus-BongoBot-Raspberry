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


class PlayBongoPwm : public Action{
	private:
		
		osc::int64 messageId;
		int pinOutputPort;
		osc::int64 pwmValue;
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
	
		PlayBongoPwm(
					long messageId,
					int solenoid,
					float velocity,
					string instrumentOscAddres,
					string serverOscAddress,
					int serverPort,
					string serverIpAddress
					
				);
				
		~PlayBongoPwm();
		void playInstrument();
		void watingBeat();
		void sendDelay();
		void play();
};
