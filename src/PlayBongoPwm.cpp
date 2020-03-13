#include "PlayBongoPwm.h"
#define OUTPUT_BUFFER_SIZE 1024
#define DEBUG 

PlayBongoPwm::PlayBongoPwm(
						long messageId,
						int solenoid,
						float velocity,
						string instrumentOscAddres,
						string serverOscAddress,
						int serverPort,
						string serverIpAddress
					){
	//0->central que esta no pino 1; 1->lateral pino 23
	if(solenoid == 0){
		this->pinOutputPort = 1;
	}else{
		this->pinOutputPort = 23;
	}

	wiringPiSetup();		
	pinMode(0, INPUT);		
	pinMode(pinOutputPort, PWM_OUTPUT);	
	
	this->messageId = messageId;
	this->pwmValue = (int) (velocity*523+500);
	this->serverOscAddress = serverOscAddress;
	this->instrumentOscAddres = instrumentOscAddres;
	this->serverIpAddress = serverIpAddress;
	this->serverPort = serverPort;
	
	this->threadObj = NULL;
	this->threadObjWaiting = NULL; 

    
    /* Create socket for sending datagrams */
    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        printf("socket() failed");

    /* Construct local address structure */
    memset(&serverAddr, 0, sizeof(serverAddr));   /* Zero out structure */
    serverAddr.sin_family = AF_INET;                 /* Internet address family */
    serverAddr.sin_addr.s_addr = inet_addr(this->serverIpAddress.c_str());
    serverAddr.sin_port = htons(this->serverPort);      

}



PlayBongoPwm::~PlayBongoPwm(){
	if(this->threadObj != NULL){
		delete this->threadObj;
	}
	
	if(this->threadObjWaiting != NULL){
		delete this->threadObjWaiting;
	}
	
}

void PlayBongoPwm::playInstrument(){
	
	
	this->threadObjWaiting = new thread(&PlayBongoPwm::watingBeat, this);
	
	this->threadObjWaiting->detach();
	
	
	
	string mac = utils::getMacAddress();
	
	this->startTime = utils::getCurrentTimeMicros();
	
	if(mac.compare("b8:27:eb:fd:6b:df")!=0){
		delay(100);
	}
	pwmWrite(pinOutputPort, this->pwmValue);
	delay(50);
	pwmWrite(pinOutputPort, 0);		

}

void PlayBongoPwm::watingBeat(){
	#ifdef DEBUG
	std::cout<<"PlayBongoPwm::watingBeat() "<<std::endl;
	#endif
	unsigned long long ini = utils::getCurrentTimeMicros();
	long long  actionDelay = -1;
	
	while(utils::getCurrentTimeMicros() - ini < 300000) //espera no maximo .3 segundo
	{
		if(digitalRead(0) == 1)
		{	
			actionDelay = utils::getCurrentTimeMicros() - startTime;
			pwmWrite(pinOutputPort, 0);	
			break;
		}
	}


	if(actionDelay > 10000 ){
		
		char buffer[OUTPUT_BUFFER_SIZE];
		osc::OutboundPacketStream p( buffer, OUTPUT_BUFFER_SIZE );

		p << osc::BeginMessage( (serverOscAddress+"/delay"+instrumentOscAddres).c_str() )
			<< this->messageId
			<< actionDelay 
			<< osc::EndMessage;

	
		//this->transmitSocket->Send( p.Data(), p.Size() );

		if (sendto(sock, p.Data(),  p.Size() , 0, (struct sockaddr *) 
					&serverAddr, sizeof(serverAddr)) != (int)p.Size())
			printf("sendto() sent a different number of bytes than expected");
		close(sock);	

		#ifdef DEBUG
		std::cout<<"Message "<<this->messageId<<" spent "<<actionDelay<<" us"<<std::endl;
		#endif
	}else{
		#ifdef DEBUG
		std::cout<<"Timeout to message "<<this->messageId<<std::endl;
		#endif
	}
	
}

void PlayBongoPwm::sendDelay(){
	
	char buffer[OUTPUT_BUFFER_SIZE];
	osc::OutboundPacketStream p( buffer, OUTPUT_BUFFER_SIZE );
	long long actionDelay = 10000;
	p << osc::BeginMessage( (serverOscAddress+"/delay"+instrumentOscAddres).c_str() )
		<< this->messageId
		<< actionDelay 
		<< osc::EndMessage;

	/*this->mtx.lock();
	this->transmitSocket->Send( p.Data(), p.Size() );
	this->mtx.unlock();
	delay(100);
	*/
	/* Broadcast sendString in datagram to clients every 3 seconds*/
    if (sendto(sock, p.Data(),  p.Size() , 0, (struct sockaddr *) 
	   &serverAddr, sizeof(serverAddr)) != (int)p.Size())
		printf("sendto() sent a different number of bytes than expected");
	close(sock);	
	#ifdef DEBUG
	std::cout<<"Message "<<this->messageId<<" spent "<<actionDelay<<" us"<<std::endl;
	#endif
	
}

void PlayBongoPwm::play(){
	
	std::cout<<"PlayBongoPwm::play()"<<std::endl;
	playInstrument();
	//this->threadObj = new thread(&PlayBongoPwm::playInstrument, this);
	//this->threadObj = new thread(&PlayBongoPwm::sendDelay, this);
	//this->threadObj->detach();
}
