
#include "BongoBot.h"

void pqp(){
	std::cout<<"pqp"<<std::endl;
}
int main(){
	
	BongoBot *bongoBot = new BongoBot();
	
	cout<<"-"<<bongoBot->getServerIpAddress()<<"-"<<bongoBot->getServerIpAddress().length()<<endl;
	bongoBot->sendHandshake();
	while(true);
	return 0;
}
