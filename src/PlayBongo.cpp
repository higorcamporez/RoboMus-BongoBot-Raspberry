#include "PlayBongo.h"


PlayBongo::PlayBongo(){
	wiringPiSetup();			// Setup the library
	pinMode(0, OUTPUT);		// Configure GPIO0 as an output
}



PlayBongo::~PlayBongo(){

}

void PlayBongo::play(){
	std::cout<<"playBongo::play()"<<std::endl;
	digitalWrite(0, 1);
	delay(1); 
	digitalWrite(0, 0);
}
