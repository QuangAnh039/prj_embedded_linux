#include "SensorNode.h"

int port;

int main(int argc, char *argv[])
{
    port = atoi(argv[1]);
    bool ret;
    SenSorNode node1(1);
    if((ret = Create_Socket(node1)) == false)
    {
        printf("Cannot create socket\n");
        return 0;
    }
    if((ret == Request_Connect(node1)) == false)
    {
        printf("Cannot connect to server\n");
        return 0;
    }
    while (1)
    {
        node1.Measure_Temp();
        Send_Data(node1);
        sleep(2);
    }
}