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
#include <errno.h>
/* You will to add includes here */
#include <iostream>

// Included to get the support library
#include "calcLib.h"

#include "protocol.h"



/* Needs to be global, to be rechable by callback and main */
int loopCount=0;
int terminate=0;
#define MAXLINE 1024 
#define PORT 4950  

/* Call back function, will be called when the SIGALRM is raised when the timer expires. */
void checkJobbList(int signum){
  // As anybody can call the handler, its good coding to check the signal number that called it.

  //printf("Let me be, I want to sleep.\n");
  //
  //if(loopCount>20){
  //  printf("I had enough.\n");
  //  terminate=1;
  //}
  
  return;
}
void makeACalculation(calcProtocol *TheProtocol);
bool calcmessageTheSame(calcMessage *a, calcMessage *b);
void calcTheMessage(calcProtocol *Thecalculations);


int main(int argc, char *argv[]){
    //make a while
   // calcMessage thistype;
   // thistype.type = 22;
   // thistype.message = 0;
   // thistype.protocol = 17;
   // thistype.major_version = 1;
   // thistype.minor_version = 0;
   // calcMessage recvMessage;
   // calcMessage sendMessage;
   // calcProtocol sendProtocol;
   // calcProtocol recvProtocol;

   // int sockfd;
   // fd_set rset;
   // sockaddr_in serverHint, clientaddr;
   // socklen_t len;
   // 
   // if(argc >= 2){
   //     int port = std::stoi(argv[1]);
   //     std::cout << port << std::endl;
   //     serverHint.sin_port = htons(port);
   // }
   // else {
   //     serverHint.sin_addr.s_addr = htonl(INADDR_ANY);
   //     serverHint.sin_family = AF_INET;
   //     serverHint.sin_port = htons(PORT);
   // }

   // sockfd = socket(AF_INET, SOCK_DGRAM, 0);
   // bind(sockfd, (sockaddr*)&serverHint, sizeof(serverHint));
   // FD_ZERO(&rset);

   // len = sizeof(clientaddr);
   // int n = recvfrom(sockfd, &recvMessage, sizeof(calcMessage), 0,
   //     (struct sockaddr*) & clientaddr, &len);
   //if (n < 0) {
   //    std::cout << "error" << std::endl;
   //}
   //else {
   //    std::cout << "client connected" << std::endl;
   //    if (calcmessageTheSame(recvMessage, thistype)) {
   //        makeACalculation(sendProtocol);
   //        int sendOk = sendto(sockfd, &sendProtocol, sizeof(struct calcProtocol), 0, (sockaddr*)&clientaddr, sizeof(clientaddr));
   //        if (sendOk < 1) {
   //            std::cout << "cant send" << std::endl;
   //            return -1;
   //        }
   //        else {
   //            std::cout << "sent protocol with calc" << std::endl;
   //        }
   //    }
   //    else {
   //        sendto(sockfd, &sendMessage, sizeof(struct calcMessage), 0, (sockaddr*)&clientaddr, sizeof(clientaddr));
   //        std::cout << "sent error" << std::endl;
   //        return -1;
   //    }
   //    calcTheMessage(sendProtocol);
   //    n = recvfrom(sockfd, &recvProtocol, sizeof(calcProtocol), 0,
   //        (struct sockaddr*) & clientaddr, &len);
   //    if (n < 1) {
   //        std::cout << "error" << std::endl;
   //        return 0;
   //    }
   //    bool ok = false;
   //    if (recvProtocol.arith == sendProtocol.arith) {
   //        if (recvProtocol.arith <= 4 && sendProtocol.arith <= 4) {
   //            //int
   //            std::cout << "got result" << sendProtocol.inResult << std::endl;
   //            if (recvProtocol.inResult == sendProtocol.inResult) {
   //                //sendok
   //                sendMessage.message = 1;
   //                sendto(sockfd, &sendMessage, sizeof(struct calcMessage), 0, (sockaddr*)&clientaddr, sizeof(clientaddr));
   //                std::cout << "sent ok" << std::endl;
   //                ok = true;
   //            }
   //        }
   //        else if (recvProtocol.arith >= 5 && sendProtocol.arith >= 5) {
   //            //float
   //            std::cout << "got result" << sendProtocol.flResult << std::endl;
   //            if (recvProtocol.flResult == sendProtocol.flResult) {
   //                //sendok
   //                sendMessage.message = 1;
   //                sendto(sockfd, &sendMessage, sizeof(struct calcMessage), 0, (sockaddr*)&clientaddr, sizeof(clientaddr));
   //                std::cout << "sent ok" << std::endl;
   //                ok = true;
   //            }
   //        }
   //        else {
   //            std::cout << "error" << std::endl;
   //        }
   //    }
   //    else {
   //        std::cout << "error" << std::endl;
   //    }
   //    if (!ok) {
   //        sendMessage.message = 2;
   //        std::cout << "sending error" << std::endl;
   //        sendto(sockfd, &sendMessage, sizeof(struct calcMessage), 0, (sockaddr*)&clientaddr, sizeof(clientaddr));
   //    }

   //    
   //}
  /* Do more magic */


  /* 
     Prepare to setup a reoccurring event every 10s. If it_interval, or it_value is omitted, it will be a single alarm 10s after it has been set. 
  */
  struct itimerval alarmTime;
  alarmTime.it_interval.tv_sec=10;
  alarmTime.it_interval.tv_usec=10;
  alarmTime.it_value.tv_sec=10;
  alarmTime.it_value.tv_usec=10;

  /* Regiter a callback function, associated with the SIGALRM signal, which will be raised when the alarm goes of */
  signal(SIGALRM, checkJobbList);
  setitimer(ITIMER_REAL,&alarmTime,NULL); // Start/register the alarm. 

  calcMessage* thistype = new calcMessage;
  thistype->type = 22;
  thistype->message = 0;
  thistype->protocol = 17;
  thistype->major_version = 1;
  thistype->minor_version = 0;

  int maxClients = 10;
  float* fResults = new float[maxClients];
  int* iResults = new int[maxClients];
  int ID = 0;

  void* voidRecv = malloc(sizeof(calcProtocol));;
  calcProtocol* recvProtocol = new calcProtocol;
  calcProtocol* sendProtocol = new calcProtocol;
  calcMessage* sendMessage = new calcMessage;
  calcMessage* recvMessage = new calcMessage;

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

while(terminate==0){
    printf("This is the main loop, %d time.\n",loopCount);
   
    int sendOk = 0;
    int n = recvfrom(sockfd, voidRecv, sizeof(calcProtocol), 0,
        (struct sockaddr*) & clientaddr, &len);
    if (n < 0)
    {
        std::cout << "error" << std::endl;
        return -1;
    }
    if (n == sizeof(calcProtocol)) {
        //check results
        recvProtocol = (calcProtocol*)voidRecv;
        if (iResults[recvProctocol->id] != nullptr || fResults[recvProctocol->id] != nullptr) {
            if (iResults[recvProtocol->id] == recvProtocol->inResult && fResults[recvProtocol->id] == recvProtocol->flResult) {
                sendMessage->message = 1;
            }
            else {
                sendMessage->message = 2;
            }
            //send results
            sendOk = sendto(sockfd, sendMessage, sizeof(struct calcMessage), 0, (sockaddr*)&clientaddr, sizeof(clientaddr));
            if (sendOk < 1) {
                std::cout << "error" << std::endl;
            }
            else {
                std::cout << "send result from protocol" << std::endl;
            }
        }
        else {
            std::cout << "error got weird message from a client" << std::endl;
        }
    }
    else if (n == sizeof(calcMessage)) {
        recvMessage = (calcMessage*)voidRecv;
        if (calcmessageTheSame(recvMessage, thistype))
        {
            //send protocol
            makeACalculation(sendProtocol);
            sendProtocol->id = ID++;
            calcTheMessage(sendProtocol);

            fResults[sendProtocol->id] = sendProtocol->flResult;
            iResults[sendProtocol->id] = sendProtocol->inResult;

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
    sleep(1);
    loopCount++;
}

  printf("done.\n");
  close(sockfd);
  return(0);


  
}

bool calcmessageTheSame(calcMessage* a, calcMessage* b)
{
    std::cout << "see if calcmessage is the same" << std::endl;
    bool theReturn = false;
    if (a->major_version    == b->major_version &&
        a->message          == b->message &&
        a->minor_version    == b->minor_version &&
        a->protocol         == b->protocol &&
        a->type             == b->type) 
    {
        theReturn = true;
    }
    return theReturn;
}

void calcTheMessage(calcProtocol* Thecalculations)
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

void makeACalculation(calcProtocol* TheProtocol) {
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