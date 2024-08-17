#include "ConnMgr.h"


bool ConnMgr::create_sock()
{
    memset(Buff, 0, sizeof(Buff));
    memset(&server_addr, 0, sizeof(server_addr));

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0) 
    {
        perror("socket failed");
        return false;
    }
    //reuse port
    int opt = 1;
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) 
    {
        perror("setsockopt failed");
        close(listenfd);
        return false;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(7000);

    if (bind(listenfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) 
    {
        perror("bind failed");
        close(listenfd);
        return false;
    }

    if (listen(listenfd, MAXCLI) < 0) 
    {
        perror("listen failed");
        close(listenfd);
        return false;
    }
    return true;
}

void ConnMgr::capture_pack(list<string>& data, mutex& mtx)
{
    connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
    while(1)
    {
        if (connfd < 0) 
        {
            perror("accept failed");
            continue;
        }
        ssize_t bytes_read = recv(connfd, Buff, sizeof(Buff) - 1, 0);
        
        int node_id = getID();
        //***********************************************************
        if(haha == 1)
        {  
            wr_log_into_FIFO("OPEN", node_id);
            haha = 0;
        }
        //***********************************************************
        if (bytes_read == 0) 
        {
            cout << "Client disconnected" << endl;
            wr_log_into_FIFO("CLOSE", node_id);
            close(connfd);
            break;
        }
        else if (bytes_read < 0)
        {
            cerr << "Error reading from socket" << endl;
            break;
        }
        Buff[bytes_read] = '\0'; // Đảm bảo chuỗi kết thúc
        printf("Received: %s\n", Buff);
        wr_into_shared(data, mtx);
    }
    show_shared(data, mtx);
    close(listenfd);
}

void ConnMgr::wr_into_shared(list<string>& data, mutex& mtx)
{
    mtx.lock();
    data.push_front(Buff);
    cout << "Already write: " << Buff << endl;
    mtx.unlock();
}

void ConnMgr::show_shared(list<string>& data, mutex& mtx)
{
    if (data.empty()) 
    {
        cout << "The list is empty : from connect." << endl;
    } 
    else 
    {
        mtx.lock();
        // Duyệt qua tất cả các phần tử trong danh sách và in ra
        cout << "node current in list" << endl;
        for (auto& node : data) 
        {
            cout << node << endl;
        }
        mtx.unlock();
    } 
}

void ConnMgr::wr_log_into_FIFO(string which, int sensor_id)
{
    timestamp = get_current_time();
    if (flock(*fd, LOCK_EX) == -1) 
    {
        std::cerr << "Error locking FIFO\n";
        return;
    }
    else
    {
        if(which == "OPEN")
        {
            string result = '<' + to_string(sequence) + '>' + '<' + timestamp + '>' + "A sensor node with <sensorNodeID: " + to_string(sensor_id) + "> has opened a new connection\n";
            cout << "A sensor node with <sensorNodeID: " << sensor_id << "> has opened a new connection " << endl;
            if (write(*fd, result.c_str(), result.size()) == -1) {
                cerr << "Error writing to FIFO\n";
            }
            
        }
        if(which == "CLOSE")
        {
            string result = '<' + to_string(sequence) + '>' + '<' + timestamp + '>' + "The sensor node with <sensorNodeID " + to_string(sensor_id) + "> has closed the connection\n";
            cout << "The sensor node with <sensorNodeID " << sensor_id << "> has closed the connection" << endl;
            if (write(*fd, result.c_str(), result.size()) == -1) {
                cerr << "Error writing to FIFO\n";
            }
        }
        sequence++;
    }
    flock(*fd, LOCK_UN);
}