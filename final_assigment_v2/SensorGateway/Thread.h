#include <iostream>
using namespace std;

#ifndef _THREAD_H_
#define _THREAD_H_

#define BUFFERSIZE 10
class Thread {
protected:
    int ThreadID;
    char Buff[BUFFERSIZE];
public:
    Thread(int ThreadID)
    {
        this->ThreadID = ThreadID;
    }
    int getID()
    {
        return ThreadID;
    }
};
#endif
