#include <iostream>
#include "Action.h"
#include "osc/OscTypes.h"

using namespace std;

class RoboMusMessage{
	private:
		osc::uint64 timetag;
		long messageId;
		Action *action;
	
	public:
		RoboMusMessage(osc::uint64 timetag, long messageId, Action *action);
		void play(void);
		osc::uint64 getTimetag(void);
		long getMessageId(void);
};
