#include "Instrument.h"

Instrument::Instrument(string name, string oscAddress,
                      int receivePort, string ip){

	this->name = name;
    this->oscAddress = oscAddress;
    this->receivePort = receivePort;
    this->ip = ip;
    
} 

string Instrument::getName() {
	return this->name;
}

void Instrument::setName(string name) {
	this->name = name;
}

string Instrument::getOscAddress() {
	return this->oscAddress;
}

void Instrument::setOscAddress(string oscAddress) {
	this->oscAddress = oscAddress;
}

string Instrument::getServerOscAddress() {
	return this->serverOscAddress;
}

string Instrument::getServerIpAddress() {
	return this->serverIpAddress;
}

