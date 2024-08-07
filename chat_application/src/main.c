#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include "/home/quanganh/chat_application/include/my_feature.h"
#include <stdbool.h>

#define MAX_CONNECTIONS 10
#define BUFFER_SIZE 1024


connection_t connections[MAX_CONNECTIONS];
int connection_count = 0;
int listening_port;
pthread_mutex_t lock;
int yes = 1;
pthread_t input_thread;
bool oke = true;

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
    while (oke) 
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
    pthread_join(input_thread, NULL);
    return 0;
}
