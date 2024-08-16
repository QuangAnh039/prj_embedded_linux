#include "DataMgr.h"
#include <cstdlib> // For atoi and atof
#include <cstring>

void DataMgr::fetch_data(list<string>& data, mutex& mtx, condition_variable& cv)
{
    while(1)
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
}

int DataMgr::conv_str_to_data()
{
    char* ptr = strchr(Buff, '-');
    int index = atoi(Buff); 
    float value = atof(ptr + 1);
    datasen[index] = value;

    return index;
}

void DataMgr::Cal_Avg(int index)
{
    avg[index] = avg[index] + datasen[index];
}

void DataMgr::notify_temp(int index)
{
    if (avg[index] > 28)
    {
        cout << "sensorNodeID" << index << "reports it's too hot " << avg[index] << endl;
    }
    else if (avg[index] < 22)
    {
        cout << "sensorNodeID" << index << "reports it's too cold " << avg[index] << endl;    
    }
    else
    {
        cout << "sensorNodeID" << index << "reports it's normal " << avg[index] << endl;
    }
}