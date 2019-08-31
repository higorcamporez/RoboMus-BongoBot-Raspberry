
#include "Instrument.h"


int main(){
	
	Instrument *i = new Instrument("bongo","/bongo",1234,"192.168.2.5");
	
	cout<<i->getName()<<endl;
	i->setName("bongobot");
	cout<<i->getName()<<endl;
	
	return 0;
}
