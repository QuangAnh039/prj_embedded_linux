#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "LogProcess.h"
#include <iostream>
#include <fstream>  // Thư viện để làm việc với file stream
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>  // Thư viện để làm việc với file descriptor
#include <cstring>  // Thư viện để làm việc với các hàm xử lý chuỗi
#include <cerrno>   // Thư viện để sử dụng perror

using namespace std;

#define FIFO_FILE   "./myfifo"
#define LOG_FILE    "./gateway.log"

#define BUFF_SIZE   150

int main()
{
    LogProcess p2;
    cout << "Hello im child process" << endl;
    cout << "my pid is: " << p2.getPID() << "\n";

    char buff[BUFF_SIZE];

    // mkfifo(<pathname>, <permission>)
    mkfifo(FIFO_FILE, 0666);
    int fd = open(FIFO_FILE, O_RDONLY);
    if (fd == -1) {
        perror("Error opening FIFO");
        return 1;
    }

    // Mở file log để ghi
    int logFd = open(LOG_FILE, O_WRONLY | O_CREAT | O_APPEND, 0666);
    if (logFd == -1) {
        perror("Error opening log file");
        close(fd);
        return 1;
    }

    while (true) {
        ssize_t bytesRead = read(fd, buff, BUFF_SIZE - 1); // Đọc dữ liệu
        if (bytesRead > 0) {
            buff[bytesRead] = '\0'; // Đảm bảo kết thúc chuỗi
            cout << "producer message: " << buff;
            if (write(logFd, buff, bytesRead) == -1) {
                perror("Error writing to log file");
                break; // Thoát khi có lỗi
            }
        } else if (bytesRead == -1) {
            perror("Error reading from FIFO");
            break; // Thoát khi có lỗi
        }  
    }
    close(fd);
    close(logFd);

    return 0;
}