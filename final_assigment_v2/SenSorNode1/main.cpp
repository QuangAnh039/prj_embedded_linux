#include "SensorNode.h"
#include <csignal>

int port;
void signalHandler(int signal) 
{
    cout << "Program interrupted and resources cleaned up\n";
    exit(signal);
}
int main(int argc, char *argv[])
{
    signal(SIGINT, signalHandler);
    port = atoi(argv[1]);
    SenSorNode node1(1);
    if(!(Create_Socket(node1)))
    {
        cout << "Can't create socket\n";
        return 0;
    }
    if(!(Request_Connect(node1)))
    {
        cout << "Can't connect to server\n";
        return 0;
    }
    while (1)
    {
        node1.Measure_Temp();
        if(!(Send_Data(node1)))
        {
            cout << "Can't SendData to server\n";
        }
        sleep(2);
    }
}