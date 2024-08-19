#include "StorageMgr.h"
#include <string>

void StorageMgr::rd_from_shared(list<string>& data, mutex& mtx, condition_variable& cv)
{
    while(running == true)
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
            
            int node_id;
            char timestamp[20];
            float temperature;
            sscanf(Buff, "%d-%f-%19[^\n]", &node_id, &temperature, timestamp);
            string timestamp_str = string(timestamp);
        
            int try_con = 0;
            while(conn_to_datab() == false)
            {
                try_con++;
                if(try_con == 3)
                {
                    wr_log_into_FIFO("UNABLE");
                    exit(1);
                }
                sleep(10);
            }
            sto_into_datab(node_id, temperature, timestamp_str);
            delete pstmt;
            delete con;
        }
        else 
        {
            cout << "Storage: list is empty." << endl;
        }
        sleep(1);
    }
    return;
}

bool StorageMgr::create_obj()
{
    try {
        // Create driver MySQL
        // Take instance of driver
    	driver = sql::mysql::get_mysql_driver_instance();
    } catch (sql::SQLException &e) {
        cerr << "SQL Error: " << e.what() << endl;
        return false;
    }
    return true;
}

bool StorageMgr::conn_to_datab()
{
    try {
        // Connect to MYSQL
    	con = driver->connect("tcp://127.0.0.1:3306", "bill", "passpass");
    } catch (sql::SQLException &e) {
        cerr << "SQL Error: " << e.what() << endl;
        return false;
    }
    return true;
}

bool StorageMgr::sto_into_datab(int node_id, float temperature, string timestamp)
{
    try {
        // Select database
        con->setSchema("mydatabase");
        // Insert data into table
        pstmt = con->prepareStatement("INSERT INTO sensornodesv2 (node_id, temperature, timestamp) VALUES (?, ?, ?)"
        "ON DUPLICATE KEY UPDATE temperature = VALUES(temperature), timestamp = VALUES(timestamp)");

        pstmt->setInt(1, node_id);
        pstmt->setDouble(2, temperature);
        pstmt->setString(3, timestamp);

        pstmt->executeUpdate();
        cout << "Insert Data Sucessfully!" << endl;
    } catch (sql::SQLException &e) {
        cerr << "SQL Error: " << e.what() << endl;
        return false;
    }
    return true;
}


void StorageMgr::wr_log_into_FIFO(string which)
{
    timestamp = get_current_time();
    if (flock(*fd, LOCK_EX) == -1) 
    {
        std::cerr << "Error locking FIFO\n";
        return;
    }
    else
    {
        if(which == "CONNECT")
        {
            //Connection to SQL server established
            string result = '<' + to_string(sequence) + '>' + '<' + timestamp + '>' + "Connection to SQL server established" + '\n'; 
            cout << "Connection to SQL server established" << " sequence: " << sequence << endl;
            if (write(*fd, result.c_str(), result.size()) == -1) 
            {
                cerr << "Error writing to FIFO\n";
            }
        }
        if(which == "TABLE")
        {
            //New table <name-of-table> created.
            string result = '<' + to_string(sequence) + '>' + '<' + timestamp + '>' + "New table <name-of-table> created" + '\n'; 
            cout << "New table <name-of-table> created" << endl;
            if (write(*fd, result.c_str(), result.size()) == -1) 
            {
                cerr << "Error writing to FIFO\n";
            }
        }
        if(which == "LOST")
        {
            //Connection to SQL server lost.
            string result = '<' + to_string(sequence) + '>' + '<' + timestamp + '>' + "Connection to SQL server lost" + '\n'; 
            cout << "Connection to SQL server lost" << endl;
            if (write(*fd, result.c_str(), result.size()) == -1) 
            {
                cerr << "Error writing to FIFO\n";
            }
        }
        if(which == "UNABLE")
        {
            //Unable to connect to SQL server.
            string result = '<' + to_string(sequence) + '>' + '<' + timestamp + '>' + "Unable to connect to SQL server" + '\n'; 
            cout << "Unable to connect to SQL server" << endl;
            if (write(*fd, result.c_str(), result.size()) == -1)
            {
                cerr << "Error writing to FIFO\n";
            }
        }
        sequence++;
    }
    flock(*fd, LOCK_UN);
    
}