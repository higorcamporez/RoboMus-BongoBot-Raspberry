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

}
