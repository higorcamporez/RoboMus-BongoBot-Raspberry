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
	
	void stopServiceNTP(){
		system("sudo timedatectl set-ntp 0");
		system("sudo service ntp stop");
	}
	
	void NTPSynchronizer(string ntpServerIp){
		
		system(("sudo ntpdate "+ntpServerIp).c_str());
		
	}
	
	string getMacAddress()
	{
		 int fd;
		
		struct ifreq ifr;
		char iface[6];
		strcpy(iface,"wlan0");
		char *mac;
		char uc_Mac[32];
		
		fd = socket(AF_INET, SOCK_DGRAM, 0);

		ifr.ifr_addr.sa_family = AF_INET;
		strncpy((char *)ifr.ifr_name , (const char *)iface , IFNAMSIZ-1);

		ioctl(fd, SIOCGIFHWADDR, &ifr);

		close(fd);
		
		mac = (char *)ifr.ifr_hwaddr.sa_data;
		
		//display mac address
		sprintf((char *)uc_Mac,(const char *)"%.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n" , mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
		
		
		string s = uc_Mac;
		return s;
	}
}
