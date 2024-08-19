#include "Thread.h"
#include <string>
#include <list>
#include <cstring>
#include <unistd.h>
#include <mutex>
#include <condition_variable>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <iostream>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <fcntl.h>
#include <sys/file.h>
#include <unistd.h>
#include <thread>
#include <vector>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>

#ifndef _STORAGE_THREAD_H_
#define _STORAGE_THREAD_H_

class StorageMgr : public Thread {
private:
    sql::PreparedStatement *pstmt;
    sql::mysql::MySQL_Driver *driver;
    sql::Connection *con;
public:
    using Thread::Thread;

    /**
     * @brief Fetch data from Shared Resource to handle.
     * @param shared_data: Reference to Shared Resource.
     * @param mtx: Reference to mutex.
     * @param cv: Reference to condtion variable
     */
    void rd_from_shared(list<string>& shared_data, mutex& mtx, condition_variable& cv);

    /**
     * @brief Create driver MySQL and take instance of driver 
     * @return true if success, false if unsuccess.
     */ 
    bool create_obj();

    /**
     * @brief Create driver MySQL and take instance of driver 
     * @return true if success, false if unsuccess.
     */ 
    bool conn_to_datab();

    /**
     * @brief Storage data into table.
     * @param node_id: Sensor id.
     * @param temperature: temperature.
     * @param timestamp: timestamp.
     * @return true if success, false if unsuccess
     */
    bool sto_into_datab(int node_id, float temperature, string timestamp);

    /**
     * @brief Notify event for LogProcess(child).
     * @param log_message: Content log.
     */
    void wr_log_into_FIFO(string log_message);
};

#endif