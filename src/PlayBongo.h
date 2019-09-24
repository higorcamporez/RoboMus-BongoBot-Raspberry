#include "RoboMusMessage.h"
#include "utils.h"
#include <iostream>
#include <wiringPi.h>
#include "osc/OscOutboundPacketStream.h"
//#include "osc/OscReceivedElements.h"
#include "ip/UdpSocket.h"
//#include "ip/IpEndpointName.h"
#include <thread> 

class PlayBongo : public Action{
	private:
		UdpTransmitSocket *transmitSocket;
		long messageId;
		string serverOscAddress;
		string instrumentOscAddres;
		thread *threadObj;
		thread *threadObjWaiting;
		unsigned long long startTime;
	public:
	
		PlayBongo(UdpTransmitSocket *transSocket,
					long messageId,
					string serverOscAddress,
					string instrumentOscAddres
				);
				
		~PlayBongo();
		void playInstrument();
		void watingBeat();
		void play();
};
