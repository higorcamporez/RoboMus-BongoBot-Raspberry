
#include "BongoBot.h"

void pqp(){
	std::cout<<"pqp"<<std::endl;
}
int main(){
	
	BongoBot *bongoBot = new BongoBot();
	
	cout<<bongoBot->getIpAddress()<<endl;
	bongoBot->sendHandshake();
	while(true);
	return 0;
}
