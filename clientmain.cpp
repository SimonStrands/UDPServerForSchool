#include <stdio.h>
#include <stdlib.h>
/* You will to add includes here */
#include <netinet/in.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
// Included to get the support library
#include "calcLib.h"
#include "protocol.h"
#define PORT "4950"  

int main(int argc, char *argv[]){
    int sockfd;
    addrinfo hints, *servinfo, *p;
	sockaddr_storage their_addr; // connector's address information
	socklen_t sin_size;
	int rv;
	char s[INET_ADDRSTRLEN];
    
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    if ((rv = getaddrinfo("127.0.0.1", PORT, &hints, &servinfo)) != 0) {
        std::cout << "error couldn't get address" << std::endl;
    }
	for (p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			std::cout << "error cant create socket" << std::endl;
		}
		break;
	}

	if (p == NULL) {
		std::cout << "error p == null" << std::endl;
	}
	int sendOk = sendto(out, s.c_str(), s.size() + 1, 0, (sockaddr*)&server, sizeof(server));
  /* Do magic */
  

}
