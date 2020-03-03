#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/timeb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
/* You will to add includes here */
#include <iostream>

// Included to get the support library
#include "calcLib.h"

#include "protocol.h"
#define PORT 4950

bool DidSend();
void DidRecv();

void calcTheMessage(calcProtocol &Thecalculations);
//check so the message is recived
//make so port and ip is in argv
int main(int argc, char *argv[]){
	calcMessage thistype;
	thistype.type = 22;
	thistype.message = 0;
	thistype.protocol = 17;
	thistype.major_version = 1;
	thistype.minor_version = 0;
	calcMessage recvMessage;
	

    int sockfd;
	struct sockaddr_in servaddr;
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		std::cout << "cant create socket" << std::endl;
	}
	memset(&servaddr, 0, sizeof(servaddr));
	if (argc == 2) {
		int port = std::stoi(argv[2]);
		servaddr.sin_port = htons(port);
		servaddr.sin_addr.s_addr = inet_addr(argv[1]);
	}
	else if(argc >= 3){
		servaddr.sin_port = htons(PORT);
		servaddr.sin_addr.s_addr = inet_addr(argv[1]);
	}
	else {
		servaddr.sin_port = htons(PORT);
		servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	}
	servaddr.sin_family = AF_INET;
	

	//"connect"
	
	//if (select(sockfd + 1, sockfd, sockfd, NULL, ));
	int sendOk = sendto(sockfd, &thistype, sizeof(struct calcMessage), 0, (sockaddr*)&servaddr, sizeof(servaddr));
	if (sendOk < 1) 
	{
		std::cout << "cant send" << std::endl;
		return -1;
	}
	else 
	{
		std::cout << "sent this type" << std::endl;
	}
	calcProtocol* recvProtocol = new calcProtocol;
	void* p_struct = malloc(sizeof(calcProtocol));
	socklen_t len = sizeof(servaddr);

	int bytesIn = recvfrom(sockfd, p_struct, sizeof(calcProtocol), 0,
		(struct sockaddr*) & servaddr, &len);

	
	if (bytesIn == sizeof(calcProtocol)) {
		std::cout << "Ok got the right thing" << std::endl;
		recvProtocol = (calcProtocol*)p_struct;
	}
	else if (bytesIn == sizeof(calcMessage)) {
		calcMessage* FUCK = (calcMessage*)&p_struct;
		std::cout << FUCK->message << "fuck" << std::endl;
		std::cout << "ERROR: got calcMessage" << std::endl;
		return 0;
	}
	else {
		return 0;
	}
	std::cout << "got " << recvProtocol->arith << "\nf1 " << recvProtocol->flValue1 << "\nf2 " << recvProtocol->flValue2 << std::endl;
	calcTheMessage(*recvProtocol);
	if (recvProtocol->arith <= 4) {
		std::cout << "got answear " << recvProtocol->inResult << std::endl;
	}
	else {
		std::cout << "got answear " << recvProtocol->flResult << std::endl;
	}
	std::cout << "sending answear " << std::endl;
	sendOk = sendto(sockfd, recvProtocol, sizeof(struct calcProtocol), 0, (sockaddr*)&servaddr, sizeof(servaddr));
	if (sendOk < 1) {
		std::cout << "error" << std::endl;
	}

	bytesIn = recvfrom(sockfd, &recvMessage, sizeof(calcMessage), 0,
		(struct sockaddr*) & servaddr, &len);
	if (recvMessage.message == 1) {
		std::cout << "got OK" << std::endl;
	}
	else {
		std::cout << "error" << std::endl;
	}
	
  /* Do magic */
	sockfd.close();
	return 0;

}

void calcTheMessage(calcProtocol& Thecalculations)
{
	std::cout << "calculation" << std::endl;
	switch (Thecalculations.arith)
	{
	case 1:
		std::cout << "add" << std::endl;
		Thecalculations.inResult = Thecalculations.inValue1 + Thecalculations.inValue2;
		break;
	case 2:
		std::cout << "sub" << std::endl;
		Thecalculations.inResult = Thecalculations.inValue1 - Thecalculations.inValue2;
		break;
	case 3:
		std::cout << "mul" << std::endl;
		Thecalculations.inResult = Thecalculations.inValue1 * Thecalculations.inValue2;
		break;
	case 4:
		std::cout << "div" << std::endl;
		Thecalculations.inResult = Thecalculations.inValue1 / Thecalculations.inValue2;
		break;
	case 5:
		std::cout << "fadd" << std::endl;
		Thecalculations.flResult = Thecalculations.flValue1 + Thecalculations.flValue2;
		break;
	case 6:
		std::cout << "fsub" << std::endl;
		Thecalculations.flResult = Thecalculations.flValue1 - Thecalculations.flValue2;
		break;
	case 7:
		std::cout << "fmul" << std::endl;
		Thecalculations.flResult = Thecalculations.flValue1 * Thecalculations.flValue2;
		break;
	case 8:
		std::cout << "fdiv" << std::endl;
		Thecalculations.flResult = Thecalculations.flValue1 / Thecalculations.flValue2;
		break;
	default:
		break;
	}
	std::cout << Thecalculations.inValue1 << " " << Thecalculations.inValue2 << std::endl;
}



