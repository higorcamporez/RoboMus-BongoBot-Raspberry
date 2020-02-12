#include <iostream>
#include <string.h> 
#include <arpa/inet.h> 
#include <sys/types.h>
#include <ifaddrs.h>
#include <sys/time.h>

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <net/if.h>
#include <unistd.h>

#define NTP_TIMESTAMP_DIFF (2208988800) 

using namespace std;

namespace utils{
	string getIpAddress();
	unsigned long long getCurrentTimeMillis();
	unsigned long long getCurrentTimeMicros();
	unsigned long long convertNTPtoUTC(unsigned long long timestamp);
	void NTPSynchronizer(string ntpServerIp);
	void SNTPSynchronizer(string ntpServerIp);
	void stopServiceNTP();
	string getMacAddress();
}

