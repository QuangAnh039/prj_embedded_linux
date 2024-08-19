#include "ConnMgr.h"
#include <poll.h>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
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
    vector<pollfd> fds;
    // Add socket of server into poll's monitor list
    pollfd server_pollfd = { listenfd, POLLIN, 0 };
    fds.push_back(server_pollfd);
    sensor_id.push_back(-1);
    cout << "Server is running and waiting for connections..." << endl;

    while(running)
    {
        int poll_count = poll(fds.data(), fds.size(), -1);   
        if (poll_count < 0) 
        { 
            perror("Poll error");
            break;
        }
        // Duyệt qua các file descriptors để kiểm tra sự kiện
        for (size_t i = 0; i < fds.size(); ++i) 
        {
            if (fds[i].revents & POLLIN) 
            {
                // Nếu sự kiện đến từ server socket, đó là một kết nối mới
                if (fds[i].fd == listenfd) 
                {
                    if (((connfd = accept(listenfd, (struct sockaddr *)NULL, NULL)) < 0)) 
                    {   
                        perror("Accept failed");
                        continue;
                    }
                    flag_noti = true;
                    cout << "New connection established, socket fd: " << connfd << endl;
                    
                    // Thêm socket của client mới vào danh sách của poll
                    pollfd client_pollfd = { connfd, POLLIN, 0 };
                    fds.push_back(client_pollfd);
                    sensor_id.push_back(-1);
                }
                // Nếu sự kiện đến từ một client, đó là dữ liệu gửi tới
                else 
                {
                    int client_fd = fds[i].fd;
                    memset(Buff, 0, sizeof(Buff) - 1);
                    int bytes_read = read(client_fd, Buff, sizeof(Buff) - 1);
                    Buff[bytes_read] = '\0';
                    
                    if (bytes_read > 0) 
                    {
                        int node_id;
                        sscanf(Buff, "%d", &node_id);
                        sensor_id[i] = node_id;
                        if(flag_noti == true)
                        {
                            wr_log_into_FIFO("OPEN", node_id);
                            flag_noti = false;
                        }           
                        cout << "Received:" << Buff << endl;
                        wr_into_shared(data, mtx);
                    } 
                    else 
                    {
                        // Nếu không có dữ liệu hoặc ngắt kết nối, đóng socket và xóa khỏi danh sách
                        int node_id;
                        node_id = sensor_id[i];
                        cout << "Client disconnected, socket fd: " << client_fd << endl;
                        wr_log_into_FIFO("CLOSE", node_id);
                        
                        close(client_fd);
                        sensor_id.erase(sensor_id.begin() + i);
                        fds.erase(fds.begin() + i);
                        //--i;
                    }
                }   
            }
        }
    }      
    show_shared(data, mtx);
    close(listenfd);
    return;
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
            cout << "A sensor node with <sensorNodeID: " << sensor_id << "> has opened a new connection " << "sequence" << sequence << endl;
            if (write(*fd, result.c_str(), result.size()) == -1) {
                cerr << "Error writing to FIFO\n";
            }
            
        }
        if(which == "CLOSE")
        {
            string result = '<' + to_string(sequence) + '>' + '<' + timestamp + '>' + "The sensor node with <sensorNodeID " + to_string(sensor_id) + "> has closed the connection\n";
            cout << "The sensor node with <sensorNodeID " << sensor_id << "> has closed the connection" << "sequence" << sequence << endl;
            if (write(*fd, result.c_str(), result.size()) == -1) {
                cerr << "Error writing to FIFO\n";
            }
        }
        sequence++;
    }
    flock(*fd, LOCK_UN);
}