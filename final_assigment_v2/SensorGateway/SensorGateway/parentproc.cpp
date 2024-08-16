#include <bits/stdc++.h>
#include "MainProcess.h"
#include <thread>
#include <iostream>
#include <mutex>
#include <fcntl.h> // for open
#include <unistd.h> // for close, write
#include <cstring> // for strlen
#include "ConnMgr.h"
#include <list>
#include "StorageMgr.h"
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include "DataMgr.h"
#include <condition_variable>

using namespace std;
// FLAG
int flag = 0;
// FIFO file path
#define FIFO_FILE   "./myfifo"
mutex fdMutex;
condition_variable cv;

// share resource
list<string> DataCommon;

void conn_manager(int threadID) 
{
    //fdMutex.lock();
    // memset(buff, 0, 20);
    // snprintf(buff, 20, "quanganh %d", threadID);
    // //sleep(1);
    // std::cout << "Hello from thread" << threadID << std::endl;

    // int fd = open(FIFO_FILE, O_WRONLY);
    // if (fd == -1) {
    //     perror("Error opening FIFO");
    //     return;
    // }

    // if (write(fd, buff, strlen(buff) + 1) == -1) {
    //     perror("Error writing to FIFO");
    // }
    // close(fd);
    //fdMutex.unlock();
    int ret;
    ConnMgr th1(threadID);
    if((ret = th1.create_sock()) == false)
    {
        return;
    }
    th1.capture_pack(DataCommon, fdMutex);
    
}

void data_manager(int threadID)
{
    DataMgr th3(threadID);
    th3.fetch_data(DataCommon, fdMutex, cv);

}

void storage_manager(int threadID)
{
    StorageMgr th2(threadID);
    th2.rd_from_shared(DataCommon, fdMutex, cv);
}

int main()
{
    MainProcess p1;
    p1.run();
    cout << p1.getPID() << endl;

    mkfifo(FIFO_FILE, 0666);

    thread t1(conn_manager, 1);
    thread t2(storage_manager, 2);
    thread t3(data_manager, 3);

    t1.join();
    t2.join();
    t3.join();

    wait(NULL);
    cout << "Im just wait childProcess" << endl;
    return 0;   
}