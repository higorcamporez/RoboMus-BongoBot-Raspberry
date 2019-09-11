#include "RoboMusMessage.h"

RoboMusMessage::RoboMusMessage(unsigned long long timetag,
								long messageId, 
								Action *action){
	this->timetag = timetag;
	this->messageId = messageId;
	this->action = action;

}

RoboMusMessage::~RoboMusMessage(){
	delete this->action;
}

void RoboMusMessage::play(void){
	//std::cout<<"RoboMusMessage::play(void)";
	this->action->play();
}

unsigned long long RoboMusMessage::getTimetag(void){
	return this->timetag;
}

long RoboMusMessage::getMessageId(void){
	return this->messageId;
}
