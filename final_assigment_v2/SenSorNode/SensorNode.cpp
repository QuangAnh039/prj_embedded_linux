#include "SensorNode.h"

SenSorNode::SenSorNode(uint8_t id)
{
    SenSorID = id;
}
SenSorNode::~SenSorNode()
{
    close(com.sockfd);
}

bool Create_Socket(SenSorNode &node)
{
    memset(&node.com.server_addr, 0, sizeof(node.com.server_addr));
    node.com.sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(node.com.sockfd < 0) {
        perror("Socket failed");
        return false;
    }
    return true;
}

bool Request_Connect(SenSorNode &node)
{
    node.com.server_addr.sin_family = AF_INET;
    node.com.server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    node.com.server_addr.sin_port = htons(port);

    if (connect(node.com.sockfd, (struct sockaddr *)&node.com.server_addr, sizeof(node.com.server_addr)) < 0) {
        perror("Connect failed");
        return false;
    }

    return true;
}
bool Send_Data(SenSorNode &node)
{
    sprintf(node.com.send_buffer, "SensorID: %d - Temp: %.2f", node.SenSorID, node.Temp);
    if (write(node.com.sockfd, node.com.send_buffer, strlen(node.com.send_buffer)) < 0) {
        perror("Send failed");
        return false;
    }
    else {
        printf("Data sent\n");
        return true;
    }
}
void SenSorNode::Measure_Temp()
{
    Temp = MIN_TEMP + (rand() / (double) RAND_MAX) * (MAX_TEMP - MIN_TEMP);
}