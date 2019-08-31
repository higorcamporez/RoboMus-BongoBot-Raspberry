#include "Instrument.h"
class BongoBot : public Instrument{

	public:
		BongoBot();
		~BongoBot();
		void sendHandshake();
}
