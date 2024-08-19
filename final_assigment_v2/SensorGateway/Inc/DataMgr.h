#include <string>
#include <list>
#include <cstring>
#include <unistd.h>
#include <mutex>
#include <condition_variable>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <list>
#include <mutex>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#include "Thread.h"

using namespace std;

#ifndef _DATA_THREAD_H_
#define _DATA_THREAD_H_

class DataMgr : public Thread {
private:
    uint8_t cnt[10] = {0};
    float datasen[10] = {0};
    float avg[10] = {0};
public:
    using Thread::Thread;

    /**
     * @brief Fetch data from Shared Resource to handle.
     * @param shared_data: Reference to Shared Resource.
     * @param mtx: Reference to mutex.
     * @param cv: Reference to condtion variable
     */
    void fetch_data(list<string>& shared_data, mutex& mtx, condition_variable& cv);

    /**
     * @brief Take data from sensornode's newsletter, include: temp, sen_id.
     * @return sensor_id.
     */
    int conv_str_to_data();

    /**
     * @brief Calculate average temperature.
     * @param id: sensor_id.
     */
    void Cal_Avg(int id);

    /**
     * @brief Notify average temperature of sensor id currently.
     * @param id: sensor_id.
     */
    void notify_temp(int id);

    /**
     * @brief Notify event for LogProcess(child).
     * @param log_message: Content log.
     * @param fifo_fd: File descriptor of FIFO.
     * @param temp: average temperature currently
     */
    void wr_log_into_FIFO(string log_message, int fifo_fd, float temp);
};

#endif