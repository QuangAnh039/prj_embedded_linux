#include "Thread.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <list>
#include <mutex>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/file.h>
#include <unistd.h>
#include <thread>
#include <vector>
#include <sys/types.h>
#ifndef _CON_MGR_H_
#define _CON_MGR_H_

#define MAXCLI  10
class ConnMgr : public Thread {
private:
    int listenfd = -1;
    int connfd = -1;
    int haha = 1; //temp
    struct sockaddr_in server_addr;
public:
    using Thread::Thread;
    bool create_sock();
    void capture_pack(list<string>&, mutex&);
    void wr_into_shared(list<string>&, mutex&);
    void show_shared(list<string>&, mutex&);
    void wr_log_into_FIFO(string, int);
};

#endif