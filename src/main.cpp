
#include "BongoBot.h"


int main(){
	
	BongoBot *bongoBot = new BongoBot();
	
	cout<<bongoBot->getIpAddress()<<endl;
	bongoBot->sendHandshake();
	return 0;
}
