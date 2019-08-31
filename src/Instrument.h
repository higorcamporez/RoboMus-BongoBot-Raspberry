#include <iostream>

using namespace std;

class Instrument{
	protected:
		string name; // nome do instrumento   
		int polyphony; // quantidade de notas
		string oscAddress; //endereço do OSC do instrumento
		string serverOscAddress; //endereço do OSC do instrumento
		string serverName;
		string serverIpAddress; // endereco do servidor
		int sendPort; // porta para envio msgOSC
		int receivePort; // porta pra receber msgOSC
		string familyType; //tipo do instrumento
		string specificProtocol; //procolo especifico do robo
		string ip;
	
	public:
		Instrument(string name, string OscAddress,
                      int receivePort, string ip);
		string getName();
		void setName(string name);
		string getOscAddress();
		void setOscAddress(string myOscAddress);
		string getServerOscAddress();
		string getServerIpAddress();
		
};
