#ifndef _FEATURE_H_
#define _FEATURE_H_

#include <arpa/inet.h>
#include <pthread.h>

#define MAX_CONNECTIONS 10
#define BUFFER_SIZE 1024

typedef struct 
{
    int sock;
    struct sockaddr_in address;
} connection_t;

extern connection_t connections[MAX_CONNECTIONS];
extern pthread_mutex_t lock;
extern int listening_port;
extern int connection_count;
extern int yes;

extern void *handle_connection(void *ptr);
extern void *input_handler(void *ptr);
extern void list_connections();
extern void terminate_connection(int id);
extern void send_message(int id, const char *message);
extern void show_myip();
extern void show_myport();
extern void connect_to_peers(const char *ip, int ports);

#endif