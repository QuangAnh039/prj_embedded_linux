#include <thread>
#include <iostream>
#include <mutex>
#include <fcntl.h> 
#include <unistd.h> 
#include <cstring> 
#include <list>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <condition_variable>
#include <csignal>

#include "MainProcess.h"
#include "ConnMgr.h"
#include "DataMgr.h"
#include "StorageMgr.h"

using namespace std;
// FLAG
int flag = 0;
int sequence = 0;
bool running = true;
pid_t child_pid;

mutex fdMutex;
condition_variable cv;

// share resource
list<string> DataCommon;

void signalHandler(int signum) 
{
    cout << "MainProcess end...\n";
    running = false;

    if (kill(child_pid, SIGINT) == -1) 
    {
        perror("Error sending SIGINT");
        return;
    }
}

void conn_manager(int threadID, int *fds) 
{
    ConnMgr th1(threadID);
    th1.fd = fds;
    if(!(th1.create_sock()))
    {
        return;
    }
    th1.capture_pack(DataCommon, fdMutex);
}

void data_manager(int threadID, int *fds)
{
    DataMgr th2(threadID);
    th2.fd = fds;
    th2.fetch_data(DataCommon, fdMutex, cv);
}

void storage_manager(int threadID, int *fds)
{
    StorageMgr th3(threadID);
    th3.fd = fds;
    if(!(th3.create_obj()))
    {
        return;
    }
    th3.wr_log_into_FIFO("CONNECT");
    th3.rd_from_shared(DataCommon, fdMutex, cv);
}

int main()
{
    signal(SIGINT, signalHandler);
    MainProcess p1;
    p1.run();
    cout << "MainProcess pid is: " << p1.getPID() << endl;
    child_pid = p1.childpid;

    mkfifo(FIFO_FILE, 0666);

    int fd = open(FIFO_FILE, O_WRONLY);
    if (fd == -1) 
    {
        cerr << "Error opening FIFO\n";
        return 1;
    }

    thread t1(conn_manager, 1, &fd);
    thread t2(data_manager, 2, &fd);
    thread t3(storage_manager, 3, &fd);

    t1.join();
    t2.join();
    t3.join();

    close(fd);
    wait(NULL);
    cout << "SensorGateway Close\n";
    return 0;   
}