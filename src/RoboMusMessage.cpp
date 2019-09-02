#include "RoboMusMessage.h"

RoboMusMessage::RoboMusMessage(osc::uint64 timetag,
								long messageId, 
								Action *action){
	this->timetag = timetag;
	this->messageId = messageId;
	this->action = action;

}
		
void RoboMusMessage::play(void){
	this->action->play();
}

osc::uint64 RoboMusMessage::getTimetag(void){
	return this->timetag;
}

long RoboMusMessage::getMessageId(void){
	return this->messageId;
}
