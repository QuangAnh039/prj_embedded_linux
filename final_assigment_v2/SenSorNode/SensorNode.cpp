#include "SensorNode.h"

SenSorNode::SenSorNode(uint8_t id)
{
    SenSorID = id;
}
SenSorNode::~SenSorNode()
{
    close(com.sockfd);
}
/**
 * @brief Create IPC Socket
 *
 * This function attempts to create a socket with config is TCP protocol
 * Communicate with server in LAN
 *
 * @param &node reference to sensornode object
 *
 * @return true if success create, false if create failed
 */
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

/**
 * @brief Request connect to server
 *
 * This function attempts connect to server with addr server is addr local
 * Use port take from argv
 *
 * @param &node reference to sensornode object
 *
 * @return true if successly connect, false if connect failed
 */
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

/**
 * @brief Send temperature to server
 *
 * This function attempts send message to server, message include ID of sensor, Temp currently  
 *
 * @param &node reference to sensornode object
 *
 * @return true if successly send, false if send failed
 */
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
/**
 * @brief Measuring temperature
 *
 * This function update temperature currently  
 *
 * @param void
 *
 * @return void
 */
void SenSorNode::Measure_Temp()
{
    Temp = MIN_TEMP + (rand() / (double) RAND_MAX) * (MAX_TEMP - MIN_TEMP);
}