#include "utils.h"

namespace utils{
	string getIpAddress(){
		string ipAddress="Unable to get IP Address";
		struct ifaddrs *interfaces = NULL;
		struct ifaddrs *temp_addr = NULL;
		int success = 0;
		// retrieve the current interfaces - returns 0 on success
		success = getifaddrs(&interfaces);
		if (success == 0) {
			// Loop through linked list of interfaces
			temp_addr = interfaces;
			while(temp_addr != NULL) {
				if(temp_addr->ifa_addr->sa_family == AF_INET) {
					// Check if interface is en0 which is the wifi connection on the iPhone
					if(strcmp(temp_addr->ifa_name, "wlan0") == 0){
						ipAddress=inet_ntoa(((struct sockaddr_in*)temp_addr->ifa_addr)->sin_addr);
					}
				}
				temp_addr = temp_addr->ifa_next;
			}
		}
		// Free memory
		freeifaddrs(interfaces);
		//cout<<ipAddress;
		return ipAddress;
	}
	
	unsigned long long getCurrentTimeMillis(){
		struct timeval tv;

		gettimeofday(&tv, NULL);

		unsigned long long millisecondsSinceEpoch =
		(unsigned long long)(tv.tv_sec) * 1000 +
		(unsigned long long)(tv.tv_usec) / 1000;
		
		return millisecondsSinceEpoch;
	}
	
	unsigned long long getCurrentTimeMicros(){
		struct timeval tv;

		gettimeofday(&tv, NULL);

		unsigned long long microsecondsSinceEpoch =
		(unsigned long long)(tv.tv_sec) * 1000000 +
		(unsigned long long)(tv.tv_usec);
		
		return microsecondsSinceEpoch;
	}

	unsigned long long convertNTPtoUTC(unsigned long long timestamp){
		unsigned long long t =  (timestamp)>>32;
		t = t - NTP_TIMESTAMP_DIFF;
		unsigned long us =  ((unsigned long)(timestamp&0xffffffff));
		us = ((double)us/0xffffffff)*1000000;
		long long time = t*1000000 + us;
		return time;
	}
	
	void NTPSynchronizer(string ntpServerIp){
		system("sudo timedatectl set-ntp 0");
		
		system(("sudo ntpdate "+ntpServerIp).c_str());
		
	}
}
