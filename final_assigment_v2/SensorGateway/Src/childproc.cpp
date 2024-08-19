#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "LogProcess.h"
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <cerrno>
#include <csignal>

using namespace std;

#define FIFO_FILE   "./myfifo"
#define LOG_FILE    "./gateway.log"

#define BUFF_SIZE   150
bool running = true;
void signalHandler(int signum) 
{
    cout << "LogProcess end..." << endl;
    running = false;
}

int main()
{
    signal(SIGINT, signalHandler);
    LogProcess logpro;
    cout << "LogProcess PID is: " << logpro.getPID() << "\n";

    char buff[BUFF_SIZE];

    // mkfifo(<pathname>, <permission>)
    mkfifo(FIFO_FILE, 0666);
    int fd = open(FIFO_FILE, O_RDONLY);
    if (fd == -1) 
    {
        perror("Error opening FIFO");
        return 1;
    }

    int logFd = open(LOG_FILE, O_WRONLY | O_CREAT | O_APPEND, 0666);
    if (logFd == -1) 
    {
        perror("Error opening log file");
        close(fd);
        return 1;
    }

    while (running) 
    {
        ssize_t bytesRead = read(fd, buff, BUFF_SIZE - 1);
        if (bytesRead > 0) 
        {
            buff[bytesRead] = '\0';
            cout << "producer message: " << buff;
            if (write(logFd, buff, bytesRead) == -1) 
            {
                perror("Error writing to log file");
                break;
            }
        } 
        else
        {
            perror("Error reading from FIFO");
            break;
        } 
    }
    close(fd);
    close(logFd);
    cout << "LogProcess End!!!\n";
    return 0;
}