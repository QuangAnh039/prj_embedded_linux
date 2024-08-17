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
    void rd_from_shared(list<string>&, mutex&, condition_variable&);
    bool create_obj();
    bool conn_to_datab();
    bool sto_into_datab(int, float, string);
    void wr_log_into_FIFO(string);
    void free_ptr();
};

#endif