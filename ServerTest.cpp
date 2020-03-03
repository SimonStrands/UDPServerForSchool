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

void makeACalculation(calcProtocol *TheProtocol);
bool calcmessageTheSame(calcMessage *a, calcMessage *b);
void calcTheMessage(calcProtocol *Thecalculations);

int main(){
    calcMessage *thistype = new calcMessage;
    thistype->type = 22;
    thistype->message = 0;
    thistype->protocol = 17;
    thistype->major_version = 1;
    thistype->minor_version = 0;

    float* fResults = new float[5];
    int* iResults = new int[5];
    int ID = 0;

    void* voidRecv = malloc(sizeof(calcProtocol));;
    calcProtocol *recvProtocol = new calcProtocol;
    calcProtocol *sendProtocol = new calcProtocol;
    calcMessage *sendMessage = new calcMessage;
    calcMessage *recvMessage = new calcMessage;

    int sockfd;
    fd_set rset;
    sockaddr_in serverHint, clientaddr;
    socklen_t len;
    if (argc >= 2) {
        int port = std::stoi(argv[1]);
        std::cout << port << std::endl;
        serverHint.sin_port = htons(port);
    }
    else {
        serverHint.sin_addr.s_addr = htonl(INADDR_ANY);
        serverHint.sin_family = AF_INET;
        serverHint.sin_port = htons(PORT);
    }
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    bind(sockfd, (sockaddr*)&serverHint, sizeof(serverHint));

    len = sizeof(clientaddr);

    while (terminate == 0)
    {
        printf("This is the main loop, %d time.\n", loopCount);
        int sendOk = 0;
        int n = recvfrom(sockfd, voidRecv, sizeof(calcProtocol), 0,
            (struct sockaddr*) & clientaddr, &len);
        if (n < 0)
        {
            std::cout << "error" << std::endl;
            return -1;
        }
        if (n = sizeof(calcProtocol)) {
            //check results
            recvProtocol = (calcProtocol*)voidRecv;
            calcTheMessage(*recvProtocol);
            //send results
            sendOk = sendto(sockfd, recvProtocol, sizeof(struct calcProtocol), 0, (sockaddr*)&servaddr, sizeof(servaddr));
            if (sendOk < 1) {
                std::cout << "error" << std::endl;
            }
            else {
                std::cout << "send result from protocol" << std::endl;
            }
        }
        else if (n = sizeof(calcMessage)) {
            recvMessage = (calcProtocol*)voidRecv;
            if (calcmessageTheSame(recvMessage, thistype))
            {
                //send protocol
                makeACalculation(sendProtocol);
                sendProtocol->id = ID++;
                std::cout << "Sending to ID: " << ID << std::endl;
                sendOk = sendto(sockfd, sendProtocol, sizeof(struct calcProtocol), 0, (sockaddr*)&clientaddr, sizeof(clientaddr));
                if (sendOk < 1) {
                    std::cout << "cant send" << std::endl;
                    return -1;
                }
                else {
                    std::cout << "sent protocol with calc" << std::endl;
                }
            }
            else 
            {
                sendMessage->type = 2; 
                sendMessage->message = 2; 
                sendMessage->major_version = 1;
                sendMessage->minor_version = 0;
                sendto(sockfd, sendMessage, sizeof(struct calcMessage), 0, (sockaddr*)&clientaddr, sizeof(clientaddr));
                std::cout << "sent error" << std::endl;
                return -1;
            }
        }
    }
    close(sockfd);
    return 0;
}
bool calcmessageTheSame(calcMessage *a, calcMessage *b)
{
    std::cout << "see if calcmessage is the same" << std::endl;
    bool theReturn = false;
    if (a->major_version == b->major_version &&
        a->message == b->message &&
        a->minor_version == b->minor_version &&
        a->protocol == b->protocol &&
        a->type == b->type)
    {
        theReturn = true;
    }
    return theReturn;
}

void calcTheMessage(calcProtocol * Thecalculations)
{
    switch (Thecalculations->arith)
    {
    case 1:
        std::cout << "add" << std::endl;
        Thecalculations->inResult = Thecalculations->inValue1 + Thecalculations->inValue2;
        break;
    case 2:
        std::cout << "sub" << std::endl;
        Thecalculations->inResult = Thecalculations->inValue1 - Thecalculations->inValue2;
        break;
    case 3:
        std::cout << "mul" << std::endl;
        Thecalculations->inResult = Thecalculations->inValue1 * Thecalculations->inValue2;
        break;
    case 4:
        std::cout << "div" << std::endl;
        Thecalculations->inResult = Thecalculations->inValue1 / Thecalculations->inValue2;
        break;
    case 5:
        std::cout << "fadd" << std::endl;
        Thecalculations->flResult = Thecalculations->flValue1 + Thecalculations->flValue2;
        break;
    case 6:
        std::cout << "fsub" << std::endl;
        Thecalculations->flResult = Thecalculations->flValue1 - Thecalculations->flValue2;
        break;
    case 7:
        std::cout << "fmul" << std::endl;
        Thecalculations->flResult = Thecalculations->flValue1 * Thecalculations->flValue2;
        break;
    case 8:
        std::cout << "fdiv" << std::endl;
        Thecalculations->flResult = Thecalculations->flValue1 / Thecalculations->flValue2;
        break;
    default:
        break;
    }
    std::cout << Thecalculations->inValue1 << " " << Thecalculations->inValue2 << std::endl;
}

void makeACalculation(calcProtocol * TheProtocol) {
    srand(time(NULL));
    int a = rand() % 8 + 1;
    TheProtocol->arith = a;
    float x = (float)(rand() % 1000 + 1) / 10.0f;
    float y = (float)(rand() % 1000 + 1) / 10.0f;
    TheProtocol->flValue1 = x;
    TheProtocol->flValue2 = y;
    TheProtocol->inValue1 = (int)x;
    TheProtocol->inValue2 = (int)y;

}