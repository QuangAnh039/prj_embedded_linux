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

int main() {
    int listenfd = -1;
    int connfd = -1;
    struct sockaddr_in server_addr;
    char recv_buffer[1024];
    
    time_t ticks;
    memset(recv_buffer, 0, sizeof(recv_buffer));
    memset(&server_addr, 0, sizeof(server_addr));

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Thiết lập tùy chọn tái sử dụng cổng
    int opt = 1;
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt failed");
        close(listenfd);
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(7000);

    if (bind(listenfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        close(listenfd);
        exit(EXIT_FAILURE);
    }

    if (listen(listenfd, 10) < 0) {
        perror("listen failed");
        close(listenfd);
        exit(EXIT_FAILURE);
    }

    connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
    while(1) {
        
        if (connfd < 0) {
            perror("accept failed");
            continue;
        }
        
        memset(recv_buffer, 0, sizeof(recv_buffer)); // Đặt lại buffer
        ssize_t bytes_read = read(connfd, recv_buffer, sizeof(recv_buffer) - 1);
        if (bytes_read < 0) {
            perror("read failed");
        } else {
            recv_buffer[bytes_read] = '\0'; // Đảm bảo chuỗi kết thúc
            printf("%s\n", recv_buffer);
        }
    }

    close(listenfd);
    return 0;
}
