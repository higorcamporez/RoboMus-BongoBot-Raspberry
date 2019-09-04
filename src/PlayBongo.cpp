#include "PlayBongo.h"


PlayBongo::PlayBongo(){

}



PlayBongo::~PlayBongo(){

}

void PlayBongo::play(){
	std::cout<<"playBongo::play()"<<std::endl;
	digitalWrite(0, 1);
	delay(1); 
	digitalWrite(0, 0);
}
