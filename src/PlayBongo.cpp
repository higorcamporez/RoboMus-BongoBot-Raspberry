#include "PlayBongo.h"
#define OUTPUT_BUFFER_SIZE 1024
#define DEBUG 

PlayBongo::PlayBongo(UdpTransmitSocket *transSocket,
					long messageId,
					string serverOscAddress,
					string instrumentOscAddres
				){
					
	wiringPiSetup();			// Setup the library
	pinMode(0, OUTPUT);		// Configure GPIO0 as an output
	pinMode(1, INPUT);	
	
	this->transmitSocket = transSocket;
	this->messageId = messageId;
	this->serverOscAddress = serverOscAddress;
	this->instrumentOscAddres = instrumentOscAddres;
	this->threadObj = NULL;
	this->threadObjWaiting = NULL; 
	
	
}



PlayBongo::~PlayBongo(){
	if(this->threadObj != NULL){
		delete this->threadObj;
	}
	
	if(this->threadObjWaiting != NULL){
		delete this->threadObjWaiting;
	}

}

void PlayBongo::playInstrument(){
	
	
	this->startTime = utils::getCurrentTimeMicros();
	
	this->threadObjWaiting = new thread(&PlayBongo::watingBeat, this);
	
	this->threadObjWaiting->detach();
	
	//this->watingBeat();
	
	digitalWrite(0, 1);
	delay(30); 
	digitalWrite(0, 0);		
	
}

void PlayBongo::watingBeat(){
	#ifdef DEBUG
	std::cout<<"PlayBongo::watingBeat() "<<std::endl;
	#endif
	unsigned long long ini = utils::getCurrentTimeMicros();
	long long  delay = -1;
	
	while(utils::getCurrentTimeMicros() - ini < 2000000) //espera no maximo um segundo
	{
		if(digitalRead(1) == 1)
		{	
			delay = utils::getCurrentTimeMicros() - startTime;
			break;
		}
	}

	
	if(delay > 0 ){
		
		char buffer[OUTPUT_BUFFER_SIZE];
		osc::OutboundPacketStream p( buffer, OUTPUT_BUFFER_SIZE );

		p << osc::BeginMessage( (serverOscAddress+"/delay"+instrumentOscAddres).c_str() )
			<< this->messageId
			<< delay 
			<< osc::EndMessage;


		this->transmitSocket->Send( p.Data(), p.Size() );
		#ifdef DEBUG
		std::cout<<"Message "<<this->messageId<<" spent "<<delay<<" us"<<std::endl;
		#endif
	}else{
		#ifdef DEBUG
		std::cout<<"Timeout to message "<<this->messageId<<std::endl;
		#endif
	}
	
}

void PlayBongo::play(){
	
	std::cout<<"playBongo::play()"<<std::endl;
	this->threadObj = new thread(&PlayBongo::playInstrument, this);
	this->threadObj->detach();
}
