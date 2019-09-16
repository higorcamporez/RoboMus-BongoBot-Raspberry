#include "Instrument.h"
//#include "RoboMusMessage.h"
#include "PlayBongo.h"
#include "utils.h"
#include "osc/OscOutboundPacketStream.h"
#include "osc/OscReceivedElements.h"
#include "osc/OscPacketListener.h"
#include "ip/UdpSocket.h"
#include "ip/IpEndpointName.h"
#include <iostream>
#include <vector>
#include <thread>  
#include <mutex>  
#include <list>         
#include <unistd.h>

using namespace std;

class BongoBot : public Instrument, public osc::OscPacketListener {
	private:
		list<RoboMusMessage*> *messages;
		RoboMusMessage *nextRoboMusMessage;
		std::mutex mtx;
		int coutLostMsgs;
		int coutMsgsArraivedLate;
	public:
		BongoBot();
		~BongoBot();
		void sendHandshake();
		void receiveHandshake(osc::ReceivedMessageArgumentStream args);
		void ProcessMessage( const osc::ReceivedMessage& m, 
				const IpEndpointName& remoteEndpoint );
		void ProcessBundle( const osc::ReceivedBundle& b,
				const IpEndpointName& remoteEndpoint );
		void playBongo(osc::ReceivedMessageArgumentStream args);
		void insertMessage(RoboMusMessage*);
		void messageController();
		RoboMusMessage* getNextMessage();
		void syncTime();
		void timeSynchronizer();
		
		
};
