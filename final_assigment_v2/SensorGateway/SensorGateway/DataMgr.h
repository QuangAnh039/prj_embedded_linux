#include "Thread.h"
#include <string>
#include <list>
#include <cstring>
#include <unistd.h>
#include <mutex>
#include <condition_variable>

using namespace std;

#ifndef _DATA_THREAD_H_
#define _DATA_THREAD_H_

class DataMgr : public Thread {
private:
    int cnt[10] = {0};
    float datasen[10] = {0};
    float avg[10] = {0};
public:
    using Thread::Thread;
    void fetch_data(list<string>&, mutex&, condition_variable&);
    int conv_str_to_data();
    void Cal_Avg(int);
    void notify_temp(int);
};

#endif