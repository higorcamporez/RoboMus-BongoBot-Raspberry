#include "PlayBongo.h"
#define OUTPUT_BUFFER_SIZE 1024
#define DEBUG 

PlayBongo::PlayBongo(
						long messageId,
						string instrumentOscAddres,
						string serverOscAddress,
						int serverPort,
						string serverIpAddress
					){
					
	wiringPiSetup();			// Setup the library
	pinMode(0, OUTPUT);		// Configure GPIO0 as an output
	pinMode(1, INPUT);	
	
	this->messageId = messageId;
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



PlayBongo::~PlayBongo(){
	if(this->threadObj != NULL){
		delete this->threadObj;
	}
	
	if(this->threadObjWaiting != NULL){
		delete this->threadObjWaiting;
	}
	
}

void PlayBongo::playInstrument(){
	
	
	this->threadObjWaiting = new thread(&PlayBongo::watingBeat, this);
	
	this->threadObjWaiting->detach();
	
	//this->watingBeat();
	
	this->startTime = utils::getCurrentTimeMicros();

	if("b8:27:eb:fd:6b:df" != utils::getMacAddress()){
		delay(100);
	}
	digitalWrite(0, 1);
	delay(30);
	digitalWrite(0, 0);		
	
}

void PlayBongo::watingBeat(){
	#ifdef DEBUG
	std::cout<<"PlayBongo::watingBeat() "<<std::endl;
	#endif
	unsigned long long ini = utils::getCurrentTimeMicros();
	long long  actionDelay = -1;
	
	while(utils::getCurrentTimeMicros() - ini < 50000) //espera no maximo um segundo
	{
		if(digitalRead(1) == 0)
		{	
			actionDelay = utils::getCurrentTimeMicros() - startTime;
			break;
		}
	}


	if(actionDelay > 0 ){
		
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

void PlayBongo::sendDelay(){
	
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

void PlayBongo::play(){
	
	std::cout<<"playBongo::play()"<<std::endl;
	playInstrument();
	//this->threadObj = new thread(&PlayBongo::playInstrument, this);
	//this->threadObj = new thread(&PlayBongo::sendDelay, this);
	//this->threadObj->detach();
}
