#ifndef SYSINFO_H
#define SYSINFO_H

#include <QString>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <memory>
#include <regex>
#include <ctime>
#include <iostream>
#include <sys/utsname.h>

const auto WM_SYS_HW_DIR {"/sys/class/dmi/id/board_serial"};
const auto WM_SYS_CPU_DIR {"/proc/cpuinfo"};
const auto WM_SYS_MEM_DIR {"/proc/meminfo"};
const auto WM_SYS_UPTIME {"/proc/uptime"};
// const auto WM_OS_NAME {"/etc/lsb-release"}; // Ubuntu
const auto WM_OS_NAME {"/etc/os-release"}; // Debian
const auto UNKNOWN_VALUE { " " };

QString getCpuName();

QString getCpuCores();

QString getCpuMHz();

QString getSeiral();

std::vector<QString> getMemory();

std::vector<QString> getOsInfo();

QString getSerialNumber();

QString getUptime();

QString getTimestamp();



#endif // SYSINFO_H
