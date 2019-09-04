#include "RoboMusMessage.h"
#include <iostream>
#include <wiringPi.h>

class PlayBongo : public Action{
	public:
		PlayBongo();
		~PlayBongo();
		void play();
};
