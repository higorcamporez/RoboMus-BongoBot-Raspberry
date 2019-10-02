
#include "BongoBot.h"
#include <sys/resource.h>
#include <signal.h>

void my_handler(int s){
           printf("Caught signal %d\n",s);
           exit(1); 

}

int main(){
	int which = PRIO_PROCESS;
	id_t pid;
	int priority = -20;
	//int ret;

	pid = getpid();
	setpriority(which, pid, priority);
	
	signal (SIGINT,my_handler);
	
	


	BongoBot *bongoBot = new BongoBot();
	
	//cout<<"-"<<bongoBot->getServerIpAddress()<<"-"<<bongoBot->getServerIpAddress().length()<<endl;
	//bongoBot->sendHandshake();
	int a;
	cin>>a;
	
	bongoBot->closeFiles();
	delete bongoBot;
	
	return 0;
}
