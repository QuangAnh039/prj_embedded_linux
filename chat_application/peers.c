#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/types.h>
#include <ifaddrs.h>

#define MAX_CONNECTIONS 10
#define BUFFER_SIZE 1024

typedef struct 
{
    int sock;
    struct sockaddr_in address;
} connection_t;

connection_t connections[MAX_CONNECTIONS];
int connection_count = 0;
int listening_port;
pthread_mutex_t lock;
int yes = 1;

void *handle_connection(void *ptr);
void *input_handler(void *ptr);
void list_connections();
void terminate_connection(int id);
void send_message(int id, const char *message);
void show_myip();
void show_myport();
void connect_to_peers(const char *ip, int ports);

int main(int argc, char *argv[]) 
{
    if(argc != 2) 
    {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        return 1;
    }

    int port = atoi(argv[1]);
    listening_port = port;
    int server_sock;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    socklen_t sin_size = sizeof(struct sockaddr_in);

    pthread_t input_thread;

    // Initialize mutex
    if (pthread_mutex_init(&lock, NULL) != 0) 
    {
        perror("mutex init failed");
        return 1;
    }

    // Create server socket
    if ((server_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
    {
        perror("socket");
        return 1;
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);
    memset(&(server_addr.sin_zero), '\0', 8);

    if (setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) 
    {
		perror("setsockopt");
		exit(1);
	}
        
    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) 
    {
        perror("bind");
        return 1;
    }
    
    if (listen(server_sock, MAX_CONNECTIONS) == -1) 
    {
        perror("listen");
        return 1;
    }

    printf("Server: listening on port %d\n", port);
    // Start input handler
    if (pthread_create(&input_thread, NULL, input_handler, NULL) != 0) 
    {
        perror("pthread_create");
        return 1;
    }
    while (1) 
    {
        int new_sock = accept(server_sock, (struct sockaddr *)&client_addr, &sin_size);
        if (new_sock == -1) 
        {
            perror("accept");
            continue;
        }

        printf("Accepted a new connection from address %s, setup at port %d\n", inet_ntoa(client_addr.sin_addr),  ntohs(client_addr.sin_port));

        pthread_mutex_lock(&lock);
        if (connection_count < MAX_CONNECTIONS) 
        {
            connections[connection_count].sock = new_sock;
            connections[connection_count].address = client_addr;

            pthread_t thread;
            if (pthread_create(&thread, NULL, handle_connection, &connections[connection_count]) != 0) 
            {
                perror("pthread_create");
            } 
            else 
            {
                connection_count++;
            }
        }
        else
        {
            close(new_sock);
            printf("Max connections reached. New connection rejected.\n");
        }
        pthread_mutex_unlock(&lock);
    }
    return 0;
}

void *handle_connection(void *ptr) 
{
    connection_t *conn = (connection_t *)ptr;
    char buffer[BUFFER_SIZE];
    int bytes_read;
    struct sockaddr_in local_addr;
    socklen_t addr_len = sizeof(local_addr);

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
            exit(0);
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
}

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

void send_message(int id, const char *message) 
{
    if (id < 0 || id >= connection_count) 
    {
        printf("Invalid connection ID.\n");
        return;
    }

    send(connections[id].sock, message, strlen(message), 0);
    printf("Message sent to connection %d.\n", id);
}

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

void show_myport() 
{
    printf("Listening on port: %d\n", listening_port);
}

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
