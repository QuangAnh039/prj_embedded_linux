#include "sysinfo.h"
#include "os_string.h"


static void parseLineAndFillMap(const std::string& line, const std::string& separator, std::map<std::string, std::string>& systemInfo)
{
    const auto pos{line.find(separator)};

    if (pos != std::string::npos)
    {
        const auto key{Utils::trim(line.substr(0, pos), " \t\"")};
        const auto value{Utils::trim(line.substr(pos + 1), " \t\"")};
        systemInfo[key] = value;
    }
}

static bool getSystemInfo(const std::string& fileName, const std::string& separator, std::map<std::string, std::string>& systemInfo)
{
    std::string info;
    std::fstream file{fileName, std::ios_base::in};
    const bool ret{file.is_open()};

    if (ret)
    {
        std::string line;

        while (file.good())
        {
            std::getline(file, line);
            parseLineAndFillMap(line, separator, systemInfo);
        }
    }

    return ret;
}

QString getCpuName()
{
    std::string retVal { UNKNOWN_VALUE };
    std::map<std::string, std::string> systemInfo;
    getSystemInfo(WM_SYS_CPU_DIR, ":", systemInfo);
    const auto& it { systemInfo.find("model name") };

    if (it != systemInfo.end())
        retVal = it->second;
    retVal = "Name: " + retVal;

    QString ret = QString::fromStdString(retVal);

    return ret;

}

QString getCpuCores()
{
    std::string retVal = "";
    std::map<std::string, std::string> systemInfo;
    getSystemInfo(WM_SYS_CPU_DIR, ":", systemInfo);
    const auto& it { systemInfo.find("processor") };

    if (it != systemInfo.end())
        retVal = "Cores: " + it->second;

    QString ret = QString::fromStdString(retVal);

    return ret;
}

QString getCpuMHz()
{
    std::string retVal = "";
    std::map<std::string, std::string> systemInfo;
    getSystemInfo(WM_SYS_CPU_DIR, ":", systemInfo);

    const auto& it { systemInfo.find("cpu MHz") };

    if (it != systemInfo.end())
        retVal = "MHz: " + it->second;
    QString ret = QString::fromStdString(retVal);

    return ret;
}

QString getSeiral()
{
    std::string retVal = "";
    std::map<std::string, std::string> systemInfo;
    getSystemInfo(WM_SYS_CPU_DIR, ":", systemInfo);

    const auto& it { systemInfo.find("Serial") };

    if (it != systemInfo.end())
        retVal = "Serial: " + it->second;
    QString ret = QString::fromStdString(retVal);

    return ret;
}

static std::string double2String(double meminfo)
{
    std::stringstream ss;
    ss << std::setprecision(2) << std::fixed << meminfo;
    std::string ret = ss.str();
    return ret;
}

std::vector<QString> getMemory()
{
    std::map<std::string, std::string> systemInfo;
    getSystemInfo(WM_SYS_MEM_DIR, ":", systemInfo);

    std::vector<QString> ret;

    double memTotal = 0.0;
    double memFree = 0.0;

    const auto& itTotal { systemInfo.find("MemTotal") };

    if (itTotal != systemInfo.end())
    {
        memTotal = (double)(std::stoull(itTotal->second) / (1024.0 * 1024.0));
        ret.push_back(QString::fromStdString("Total: " + double2String(memTotal) + "GB"));
    }

    const auto& itFree { systemInfo.find("MemFree") };

    if (itFree != systemInfo.end())
    {
        memFree = (double)(std::stoull(itFree->second) / (1024.0 * 1024.0));
        ret.push_back(QString::fromStdString("Free: " + double2String(memFree) + "GB"));
    }

    const auto ramTotal { memTotal == 0 ? 1 : memTotal };
    int ramUsage = 100 - (100 * memFree / ramTotal);
    ret.push_back(QString::fromStdString("Usage: " + std::to_string(ramUsage) + "%"));

    return ret;

}

std::vector<QString> getOsInfo()
{
    struct utsname uts {};
    std::vector<QString> ret;
    if (uname(&uts) >= 0)
    {
        ret.push_back(QString::fromStdString(uts.machine));
        ret.push_back(QString::fromStdString(uts.release));
    }
    else
    {
        ret.push_back("");
        ret.push_back("");
    }

    std::map<std::string, std::string> systemInfo;
    getSystemInfo(WM_OS_NAME, "=", systemInfo);
    const auto& it { systemInfo.find("NAME") };
    // const auto& it { systemInfo.find("DISTRIB_DESCRIPTION") };
    if (it != systemInfo.end())
        ret.push_back(QString::fromStdString(it->second));
    else
        ret.push_back("");

    return ret;
}


QString getSerialNumber()
{
    std::string serial;
    std::fstream file {WM_SYS_HW_DIR, std::ios_base::in};

    if (file.is_open())
        file >> serial;
    else
        serial = UNKNOWN_VALUE;

    QString ret = QString::fromStdString(serial);

    return ret;
}

QString getTimestamp()
{
    time_t currentTime = time(nullptr);

    struct tm *localTime = localtime(&currentTime);

    int hours = localTime->tm_hour;
    int minutes = localTime->tm_min;
    int seconds = localTime->tm_sec;
    std::string ts = std::to_string(hours) + ":" +
                     (minutes < 10 ? "0" : "") + std::to_string(minutes) + ":" +
                     (seconds < 10 ? "0" : "") + std::to_string(seconds);

    QString ret = QString::fromStdString(ts);
    return ret;
}

QString getUptime()
{
    std::ifstream uptimeFile(WM_SYS_UPTIME);
    if (!uptimeFile.is_open()) {
        return UNKNOWN_VALUE;
    }

    int uptime;
    uptimeFile >> uptime;

    std::cout << uptime;

    uptimeFile.close();
    int hours = uptime / 3600;
    int minutes = (uptime % 3600) / 60;
    int seconds = uptime % 60;
    std::string tmp = std::to_string(hours) + ":" +
                      (minutes < 10 ? "0" : "") + std::to_string(minutes) + ":" +
                      (seconds < 10 ? "0" : "") + std::to_string(seconds);

    QString ret = QString::fromStdString(tmp);

    return ret;
}
