#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <sys/file.h>
#include <unistd.h>
#include <thread>
#include <vector>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>

#include "DataMgr.h"

void DataMgr::fetch_data(list<string>& data, mutex& mtx, condition_variable& cv)
{
    while(running == true)
    {
        if (!data.empty()) 
        {
            unique_lock<mutex> lock(mtx);
            strncpy(Buff, data.back().c_str(), sizeof(Buff) - 1);
            Buff[sizeof(Buff) - 1] = '\0';
            flag = 1;
            cout << "Data: already read: " << Buff << endl;
            lock.unlock();
            cv.notify_one();

            int index = conv_str_to_data();
            Cal_Avg(index);
            cnt[index]++;
            if(cnt[index] == 5)
            {
                avg[index] = avg[index] / 5;
                notify_temp(index);
                cnt[index] = 0;
                avg[index] = 0;
            }
        } 
        else 
        {
            cout << "Data: list is empty." << endl;
        }
        sleep(1);
    }
    return;
}

int DataMgr::conv_str_to_data()
{
    int node_id; 
    float temperature;
    sscanf(Buff, "%d-%f", &node_id, &temperature);
    datasen[node_id] = temperature;

    return node_id;
}

void DataMgr::Cal_Avg(int index)
{
    avg[index] = avg[index] + datasen[index];
}

void DataMgr::notify_temp(int index)
{
    if (avg[index] > 28)
    {
        cout << "sensorNodeID: " << index << " reports it's too hot " << avg[index] << endl;
        wr_log_into_FIFO("HOT", index, avg[index]);
    }
    else if (avg[index] < 22)
    {
        cout << "sensorNodeID: " << index << " reports it's too cold " << avg[index] << endl;    
        wr_log_into_FIFO("COLD", index, avg[index]);
    }
    else
    {
        cout << "sensorNodeID: " << index << " reports it's normal " << avg[index] << endl;
        wr_log_into_FIFO("NORMAL", index, avg[index]);
    }
}

void DataMgr::wr_log_into_FIFO(string which, int sensor_id, float avg)
{
    timestamp = get_current_time();
    if (flock(*fd, LOCK_EX) == -1) 
    {
        std::cerr << "Error locking FIFO\n";
        return;
    }
    else
    {
        if(which == "COLD")
        {
            //reports it’s too cold
            string result = '<' + to_string(sequence) + '>' + '<' + timestamp + '>' + "The sensor node with <sensorNodeID: " + to_string(sensor_id) + "> reports it's too cold (running avg temperature = " + to_string(avg) + ')' + '\n'; 
            cout << "The sensor node with <sensorNodeID: " << sensor_id << "> reports it's too cold (running avg temperature = " << avg << endl;
            if (write(*fd, result.c_str(), result.size()) == -1) 
            {
                cerr << "Error writing to FIFO\n";
            }
        }
        if(which == "HOT")
        {
            //reports it’s too hot
            string result = '<' + to_string(sequence) + '>' + '<' + timestamp + '>' + "The sensor node with <sensorNodeID: " + to_string(sensor_id) + "> reports it's too hot (running avg temperature = " + to_string(avg) + ')' + '\n'; 
            cout << "The sensor node with <sensorNodeID>: " << sensor_id << " reports it's too hot (running avg temperature = " << avg << endl;
            if (write(*fd, result.c_str(), result.size()) == -1) 
            {
                cerr << "Error writing to FIFO\n";
            }
        }
        if(which == "NORMAL")
        {
            string result = '<' + to_string(sequence) + '>' + '<' + timestamp + '>' + "The sensor node with <sensorNodeID: " + to_string(sensor_id) + "> reports it's normal (running avg temperature = " + to_string(avg) + ')' + '\n'; 
            cout << "The sensor node with <sensorNodeID>: " << sensor_id << " reports it's normal (running avg temperature = " << avg << endl;
            if (write(*fd, result.c_str(), result.size()) == -1) 
            {
                cerr << "Error writing to FIFO\n";
            }
            
        }
        if(which == "INVALID")
        {
            //invalid sensor node ID 
            cout << "Received sensor data with invalid sensor node ID <node-ID>" << endl;
        }
        sequence++;
    }
    flock(*fd, LOCK_UN);
}