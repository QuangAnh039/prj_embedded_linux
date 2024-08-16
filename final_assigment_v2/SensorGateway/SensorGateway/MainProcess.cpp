#include "MainProcess.h"
#include "LogProcess.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <thread>
void MainProcess::run()
{
    pid = fork();
    if(pid == 0) {
        LogProcess child;
        child.run();
        //exit(0);
    } else if (pid > 0) {
        cout << "Hello Im parent Process" << endl;
    } else {
        perror("fork");
    }
}