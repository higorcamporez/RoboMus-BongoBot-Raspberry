#include <iostream>
#include <string.h> 
#include <arpa/inet.h> 
#include <iostream>
#include <sys/types.h>
#include <ifaddrs.h>

using namespace std;

namespace utils{
	string getIpAddress();
	unsigned long long getCurrentTimeMillis();
	unsigned long long getCurrentTimeMicros();
	unsigned long long convertNTPtoUTC(unsigned long long timestamp);
}

