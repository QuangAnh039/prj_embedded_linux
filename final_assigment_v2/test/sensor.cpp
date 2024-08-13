#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <iostream>
using namespace std;

#define MIN_TEMP 18
#define MAX_TEMP 27

int main() {
    int id = 1;
    
    int sockfd = -1;
    struct sockaddr_in server_addr;
    char send_buffer[1024];
    
    
    memset(&server_addr, 0, sizeof(server_addr));

    sockfd = socket(AF_INET, SOCK_STREAM, 0); // Sử dụng AF_INET
    if (sockfd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(6000);

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
	while(1)
	{
		double Tempure = MIN_TEMP + (rand() / (double) RAND_MAX) * (MAX_TEMP - MIN_TEMP); // Sửa kiểu dữ liệu
		sprintf(send_buffer, "SensorID: %d - Temp: %.2f", id, Tempure); // Định dạng giá trị thực
		if (write(sockfd, send_buffer, strlen(send_buffer)) < 0) {
        	perror("write failed");
    	} 
		else {
        	printf("Data sent\n");
    	}
		sleep(1);
	}

    close(sockfd);
    return 0;
}
