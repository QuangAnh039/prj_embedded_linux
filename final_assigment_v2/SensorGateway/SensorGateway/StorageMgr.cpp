#include "StorageMgr.h"


void StorageMgr::rd_from_shared(list<string>& data, mutex& mtx, condition_variable& cv)
{
    while(1)
    {
        if (!data.empty()) 
        {
            unique_lock<mutex> lock(mtx);
            cv.wait(lock, [] { return flag == 1; });

            strncpy(Buff, data.back().c_str(), sizeof(Buff) - 1);
            Buff[sizeof(Buff) - 1] = '\0';

            data.pop_back();
            flag = 0;
            lock.unlock();
            cout << "Storage: already read: " << Buff << endl;
            
        } 
        else 
        {
            cout << "Storage: list is empty." << endl;
        }
        sleep(1);
    }
}