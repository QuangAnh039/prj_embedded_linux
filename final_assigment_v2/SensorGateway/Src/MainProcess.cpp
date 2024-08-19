#include "MainProcess.h"
#include "LogProcess.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <thread>
void MainProcess::run()
{
    childpid = fork();
    if(childpid == 0) 
    {
        LogProcess child;
        child.run();
    } 
    else if (childpid > 0) 
    {
        cout << "Hello Im parent Process" << endl;
    } 
    else 
    {
        perror("fork");
        exit(1);
    }
}