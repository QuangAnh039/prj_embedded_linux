#include "Thread.h"
#include <string>
#include <list>
#include <cstring>
#include <unistd.h>
#include <mutex>
#include <condition_variable>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <list>
#include <mutex>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
using namespace std;

#ifndef _DATA_THREAD_H_
#define _DATA_THREAD_H_

class DataMgr : public Thread {
private:
    
    uint8_t cnt[10] = {0};
    float datasen[10] = {0};
    float avg[10] = {0};
public:
    using Thread::Thread;
    void fetch_data(list<string>&, mutex&, condition_variable&);
    int conv_str_to_data();
    void Cal_Avg(int);
    void notify_temp(int);
    void wr_log_into_FIFO(string, int, float);
};

#endif