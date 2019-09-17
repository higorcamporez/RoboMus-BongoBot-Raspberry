#include <iostream>
#include <string.h> 
#include <arpa/inet.h> 
#include <iostream>
#include <sys/types.h>
#include <ifaddrs.h>
#include <sys/time.h>

#define NTP_TIMESTAMP_DIFF (2208988800) 

using namespace std;

namespace utils{
	string getIpAddress();
	unsigned long long getCurrentTimeMillis();
	unsigned long long getCurrentTimeMicros();
	unsigned long long convertNTPtoUTC(unsigned long long timestamp);
	void NTPSynchronizer(string ntpServerIp);
	void stopServiceNTP();
}

