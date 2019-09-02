#include "Instrument.h"
#include "osc/OscOutboundPacketStream.h"

#include "ip/UdpSocket.h"
#include "ip/IpEndpointName.h"

class BongoBot : public Instrument{

	public:
		BongoBot();
		~BongoBot();
		void sendHandshake();
};
