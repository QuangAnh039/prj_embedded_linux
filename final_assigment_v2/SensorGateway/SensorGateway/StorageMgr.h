#include "Thread.h"
#include <string>
#include <list>
#include <cstring>
#include <unistd.h>
#include <mutex>
#include <condition_variable>

#ifndef _STORAGE_THREAD_H_
#define _STORAGE_THREAD_H_

class StorageMgr : public Thread {
public:
    using Thread::Thread;
    void rd_from_shared(list<string>&, mutex&, condition_variable&);
    
};

#endif