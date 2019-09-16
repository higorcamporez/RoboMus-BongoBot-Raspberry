
#include "BongoBot.h"
#include <sys/resource.h>

void pqp(){
	std::cout<<"pqp"<<std::endl;
}
int main(){
	int which = PRIO_PROCESS;
	id_t pid;
	int priority = -20;
	//int ret;

	pid = getpid();
	setpriority(which, pid, priority);
	
	
	BongoBot *bongoBot = new BongoBot();
	
	//cout<<"-"<<bongoBot->getServerIpAddress()<<"-"<<bongoBot->getServerIpAddress().length()<<endl;
	bongoBot->sendHandshake();
	while(true);
	return 0;
}
