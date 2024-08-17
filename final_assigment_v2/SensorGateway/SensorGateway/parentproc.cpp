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
int sequence = 0;

mutex fdMutex;
condition_variable cv;

// share resource
list<string> DataCommon;

void conn_manager(int threadID, int *fds) 
{
    int ret;
    ConnMgr th1(threadID);
    th1.fd = fds;
    if((ret = th1.create_sock()) == false)
    {
        return;
    }
    th1.capture_pack(DataCommon, fdMutex);
    
}

void data_manager(int threadID, int *fds)
{
    DataMgr th3(threadID);
    th3.fd = fds;
    th3.fetch_data(DataCommon, fdMutex, cv);

}

void storage_manager(int threadID, int *fds)
{
    StorageMgr th2(threadID);
    int ret = 0;
    th2.fd = fds;
    if((ret = th2.create_obj()) == false)
    {
        return;
    }
    th2.wr_log_into_FIFO("CONNECT");
    th2.rd_from_shared(DataCommon, fdMutex, cv);

}

int main()
{
    MainProcess p1;
    p1.run();
    cout << p1.getPID() << endl;

    mkfifo(FIFO_FILE, 0666);
    int fd = open(FIFO_FILE, O_WRONLY);
    if (fd == -1) 
    {
        cerr << "Error opening FIFO\n";
        return 1;
    }

    thread t1(conn_manager, 1, &fd);
    thread t2(storage_manager, 2, &fd);
    thread t3(data_manager, 3, &fd);

    t1.join();
    t2.join();
    t3.join();

    close(fd);
    wait(NULL);

    cout << "SensorGateway close\n";
    return 0;   
}