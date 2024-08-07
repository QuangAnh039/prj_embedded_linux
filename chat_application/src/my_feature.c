#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include "/home/quanganh/chat_application/include/my_feature.h"
/**
 * @brief Connects to a peer at the specified IP address and port.
 *
 * This function attempts to create a socket connection to a peer using the provided
 * IP address and port number. If successful, the connection is added to the list
 * of active connections and a separate thread is created to handle communication
 * with the peer.
 *
 * @param ip The IP address of the peer to connect to.
 * @param port The port number of the peer to connect to.
 *
 * @return void
 */
void connect_to_peers(const char *ip, int ports)
{
    int sock;
    struct sockaddr_in server_addr;

    // create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
    {
        perror("socket");
        return;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(ports);
    inet_pton(AF_INET, ip, &server_addr.sin_addr);

    // connect to peer
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) 
    {
        perror("connect");
        close(sock);
        return;
    }

    // Add peers into list
    pthread_mutex_lock(&lock);

    if (connection_count < MAX_CONNECTIONS) 
    {
        connections[connection_count].sock = sock;
        connections[connection_count].address.sin_family = AF_INET;
        connections[connection_count].address.sin_port = htons(ports);
        inet_pton(AF_INET, ip, &connections[connection_count].address.sin_addr);

        pthread_t thread;
        if (pthread_create(&thread, NULL, handle_connection, &connections[connection_count]) != 0) 
        {
            perror("pthread_create");
        } 
        else 
        {
            connection_count++;
            printf("Connected to %s:%d\n", ip, ports);
        }
    } 
    else 
    {
        close(sock);
        printf("Max connections reached. Connection to %s:%d rejected.\n", ip, ports);
    }

    pthread_mutex_unlock(&lock);
}
/**
 * @brief Show ip of machine
 *
 * @return void
 */
void show_myip() 
{
    struct ifaddrs *ifaddr, *ifa;
    char ip[INET_ADDRSTRLEN];

    if (getifaddrs(&ifaddr) == -1) 
    {
        perror("getifaddrs");
        return;
    }

    printf("IP addresses:\n");

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) 
    {
        if (ifa->ifa_addr == NULL)
        {
            continue;
        }
        if (ifa->ifa_addr->sa_family == AF_INET) 
        {
            if (inet_ntop(AF_INET, &((struct sockaddr_in *)ifa->ifa_addr)->sin_addr, ip, sizeof(ip)) != NULL) 
            {
                printf("%s: %s\n", ifa->ifa_name, ip);
            } 
            else 
            {
                perror("inet_ntop");
            }
        }
    }
    freeifaddrs(ifaddr);
}
/**
 * @brief Show port listening of machine
 *
 * @return void
 */
void show_myport() 
{
    printf("Listening on port: %d\n", listening_port);
}
/**
 * @brief List peers are connecting
 *
 * @return void
 */
void list_connections()
{
    printf("\n*****************************************\n");
    pthread_mutex_lock(&lock);
    printf("ID\tIP\t\tPort\n");
    for(int i = 0; i < connection_count; i++)
    {
        printf("%d|\t%s\t|%d\n", i, inet_ntoa(connections[i].address.sin_addr), ntohs(connections[i].address.sin_port));
    }
    pthread_mutex_unlock(&lock);
    printf("*****************************************\n");
}
/**
 * @brief Terminate a peer follow id 
 *   
 * @param id The id of the peer is tached by queue.
 * @return void
 */
void terminate_connection(int id)
{
    if(id < 0 || id >= connection_count)
    {
        printf("Invalid connection ID.\n");
        return;
    }
    close(connections[id].sock);
    pthread_mutex_lock(&lock);
    for(int i = id; i < connection_count - 1; i++)
    {
        connections[i] = connections[i + 1];
    }
    connection_count--;
    pthread_mutex_unlock(&lock);
    printf("Connection terminated. \n");
}
/**
 * @brief Sends a message to a peer identified by the connection ID.
 *
 * This function attempts to send a message to the peer associated with the
 * specified connection ID. If the message is sent successfully, a confirmation 
 * message is printed. If an error occurs during sending, an error message is
 * printed using perror.
 *
 * @param id The ID of the connection to which the message will be sent.
 *           This ID corresponds to the index in the global connections array.
 * @param message The message to be sent to the peer.
 *
 * @return void
 */
void send_message(int id, const char *message) 
{
    if (id < 0 || id >= connection_count) 
    {
        printf("Invalid connection ID.\n");
        return;
    }
    ssize_t bytes_sent = send(connections[id].sock, message, strlen(message), 0);
    if (bytes_sent == -1)
    {
        perror("Failed to send message");
    }
    else
    {
        printf("Message sent to connection %d.\n", id);
    }
}
/**
 * @brief Handles communication with a connected peer.
 *
 * This function continuously reads data from a connected peer through the socket
 * associated with the given connection structure. It prints the received data
 * to the standard output along with the peer's IP address and port number.
 * The function terminates when the connection is closed by the peer. After
 * termination, the connection socket is closed, and the global connection count
 * is decremented. The function then exits the thread.
 *
 * @param ptr A pointer to a `connection_t` structure containing information about
 *            the connection, including the socket file descriptor and peer's
 *            address.
 *
 * @return void
 */
void *handle_connection(void *ptr) 
{
    connection_t *conn = (connection_t *)ptr;
    char buffer[BUFFER_SIZE];
    int bytes_read;

    while ((bytes_read = recv(conn->sock, buffer, BUFFER_SIZE - 1, 0)) > 0) 
    {
        buffer[bytes_read] = '\0';
        printf("Received from ip: %s port: %d - content: %s\n", inet_ntoa(conn->address.sin_addr), ntohs(conn->address.sin_port), buffer);
    }

    close(conn->sock);
    pthread_mutex_lock(&lock);
    connection_count--;
    pthread_mutex_unlock(&lock);

    pthread_exit(0);
}

/**
 * @brief Handles user input and processes commands.
 *
 * This function runs in a loop, continuously reading commands from the standard input
 * and processing them based on their content. The commands allow users to interact with
 * the chat application by displaying IP address and port, connecting to peers, listing
 * active connections, terminating connections, sending messages, or exiting the application.
 * The loop continues until the "exit" command is received. When the function detects the
 * "exit" command, it closes all active connections and then terminates the thread.
 *
 * @param ptr A pointer that is not used in this function. It is included for compatibility
 *            with the thread interface.
 *
 * @return void* This function does not return a value. It uses `pthread_exit(NULL)`
 *                to end the thread when the "exit" command is issued.
 */
void *input_handler(void *ptr) 
{
    char command[BUFFER_SIZE];
    char message[BUFFER_SIZE];
    int id;
    unsigned int ports;
    while(1)
    {
        printf("> ");
        fgets(command, BUFFER_SIZE, stdin);

        if(strncmp(command, "help", 4) == 0)
        {
            printf("Available commands: \n");
            printf("help                 - Show this help message\n");
            printf("myip                 - Display the IP address\n");
            printf("myport               - Display the listening port\n");
            printf("connect <IP> <Port>  - Connect to a peer\n");
            printf("list                 - List all connections\n");
            printf("terminate <id>       - Terminate a connection\n");
            printf("send <id> <message>  - Send a message to a connection\n");
            printf("exit                 - Close all connections and exit\n");
        }
        else if(strncmp(command, "myip", 4) == 0)
        {
            show_myip();
        }
        else if(strncmp(command, "myport", 6) == 0)
        {
            show_myport();
        }
        else if(sscanf(command, "connect %s %d", message, &ports) == 2)
        {
            connect_to_peers(message, ports);
        }
        else if(strncmp(command, "list", 4) == 0)
        {
            list_connections();
        }
        else if(sscanf(command, "terminate %d", &id) == 1)
        {
            terminate_connection(id);
        }
        else if(sscanf(command, "send %d %[^\n]", &id, message) == 2)
        {
            send_message(id, message);
        }
        else if(strncmp(command, "exit", 4) == 0)
        {
            //close all connections and exit
            for(int i = 0; i < connection_count; i++)
            {
                close(connections[i].sock);
            }
            oke = false;
            break;
        }
        else if(strncmp(command, "\n", 1) == 0)
        {
            //do nothing
        }
        else
        {
            printf("Unknown command. Type 'help' for list of commands.\n");
        }
    }
    pthread_exit(NULL);
}