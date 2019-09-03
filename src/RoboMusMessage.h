#include <iostream>
#include "Action.h"
#include "osc/OscTypes.h"

using namespace std;

class RoboMusMessage{
	private:
		unsigned long long timetag;
		long messageId;
		Action *action;
	
	public:
		RoboMusMessage(unsigned long long timetag, long messageId, Action *action);
		~RoboMusMessage();
		void play(void);
		unsigned long long getTimetag(void);
		long getMessageId(void);
};
