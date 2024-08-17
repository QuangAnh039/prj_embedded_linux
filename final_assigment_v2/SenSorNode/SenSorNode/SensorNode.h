#include <bits/stdc++.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <chrono>
#include <iomanip>  // For std::put_time
#include <ctime>    // For std::localtime
using namespace std;

#ifndef _SENSOR_NODE_H_
#define _SENSOR_NODE_H_

#define MIN_TEMP 18
#define MAX_TEMP 27

extern int port;

typedef struct communicate
{
    int sockfd = -1;
    struct sockaddr_in server_addr;
    char send_buffer[50];
} communicate;

class SenSorNode{
private:
    uint8_t SenSorID;
    float Temp;
    communicate com;
public:
    SenSorNode(uint8_t);
    ~SenSorNode();
    friend bool Create_Socket(SenSorNode &);
    friend bool Request_Connect(SenSorNode &);
    friend bool Send_Data(SenSorNode &);
    void Measure_Temp();
};


#endif