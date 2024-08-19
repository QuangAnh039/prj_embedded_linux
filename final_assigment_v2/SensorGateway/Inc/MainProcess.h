#include "Process.h"

class MainProcess : public Process {
public:
    pid_t childpid;
    void run();
};