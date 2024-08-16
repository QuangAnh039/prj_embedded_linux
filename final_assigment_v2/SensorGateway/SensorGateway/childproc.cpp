#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "LogProcess.h"
using namespace std;

#define FIFO_FILE   "./myfifo"
#define BUFF_SIZE   20

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

    while (true) {
        ssize_t bytesRead = read(fd, buff, BUFF_SIZE - 1); // Đọc dữ liệu
        if (bytesRead > 0) {
            buff[bytesRead] = '\0'; // Đảm bảo kết thúc chuỗi
            cout << "producer message: " << buff << endl;
        } if (bytesRead == -1) {
            perror("Error reading from FIFO");
            break; // Thoát khi có lỗi
        }  
    }
    close(fd);
    return 0;
}