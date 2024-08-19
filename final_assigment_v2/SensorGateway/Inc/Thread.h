#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>

using namespace std;

#ifndef _THREAD_H_
#define _THREAD_H_

#define BUFFERSIZE 50
// FIFO file path
#define FIFO_FILE   "./myfifo"
extern int flag;
extern int sequence;
extern bool running;
class Thread {
protected:
    string timestamp;
    int ThreadID;
    char Buff[BUFFERSIZE];
public:
    int *fd;
    Thread(int ThreadID)
    {
        this->ThreadID = ThreadID;
    }
    int getID()
    {
        return ThreadID;
    }
    string get_current_time()
    {
        time_t t = time(nullptr);
        
        tm* tm_ptr = localtime(&t);

        ostringstream oss;
        oss << std::put_time(tm_ptr, "%Y-%m-%d %H:%M:%S");

        return oss.str();
    }
    virtual void wr_log_into_FIFO(string){};
};
#endif
