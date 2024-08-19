#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <list>
#include <mutex>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/file.h>
#include <unistd.h>
#include <thread>
#include <vector>
#include <sys/types.h>

#include "Thread.h"

#ifndef _CON_MGR_H_
#define _CON_MGR_H_

#define MAXCLI  10

class ConnMgr : public Thread {
private:
    int listenfd = -1;
    int connfd = -1;
    vector<int> sensor_id;
    bool flag_noti;
    struct sockaddr_in server_addr;
public:
    using Thread::Thread;
    /**
     * @brief Create socket and bind to IP address 127.0.0.1 and Port 7000.
     * @return true if create success, false if unsuccess.
     */
    bool create_sock();

    /**
     * @brief Receive and handle packet data form clients.
     * @param shared_data: Reference to Shared Resource.
     * @param mtx: Reference to mutex.
     */
    void capture_pack(list<string>& shared_data, mutex& mtx);

    /**
     * @brief Insert data into shared resource.
     * @param shared_data: Reference to Shared Resource.
     * @param mtx: Reference to mutex.
     */
    void wr_into_shared(list<string>& shared_data, mutex& mtx);

    /**
     * @brief Show current node in Shared Resource.
     * @param shared_data: Reference to Shared Resource.
     * @param mtx: Reference to mutex.
     */
    void show_shared(list<string>& shared_data, mutex& mtx);
    
    /**
     * @brief Notify event for LogProcess(child).
     * @param log_message: Content log.
     * @param fifo_fd: File descriptor of FIFO.
     */
    void wr_log_into_FIFO(string log_message, int fifo_fd);
};

#endif
