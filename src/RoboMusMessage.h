#include <iostream>

#include "Action.h"


using namespace std;

class RoboMusMessage{
	private:
		uint64 timetag;
		long messageId;
		Action action;
	
	public:
		RoboMusMessage(uint64 timetag, long messageId, Action action);
		void play(void);
		uint64 getTimetag(void);
		long getMessageId(void);
}
