#include "Thread.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <list>
#include <mutex>
#ifndef _CON_MGR_H_
#define _CON_MGR_H_

#define MAXCLI  10
class ConnMgr : public Thread {
private:
    int listenfd = -1;
    int connfd = -1;
    struct sockaddr_in server_addr;
public:
    using Thread::Thread;
    bool create_sock();
    void capture_pack(list<string>&, mutex&);
    void wr_into_shared(list<string>&, mutex&);
    void show_shared(list<string>&, mutex&);
};

#endif