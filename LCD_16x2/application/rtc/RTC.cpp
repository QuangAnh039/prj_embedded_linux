#include "RTC.hpp"
#include <cstdint>
#include <chrono>
#include <string>
RTC_Object::RTC_Object(/* args */)
{

}

RTC_Object::~RTC_Object()
{

}

std::uint8_t RTC_Object::get_day()
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm* now_tm = std::localtime(&now_time);
    day = now_tm->tm_mday;
    std::string sday = std::to_string(day);
    return day;
}

std::uint8_t RTC_Object::get_month()
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm* now_tm = std::localtime(&now_time);
    month = now_tm->tm_mon + 1;
    return month;
}

std::uint8_t RTC_Object::get_year()
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm* now_tm = std::localtime(&now_time);
    year = now_tm->tm_year + 1900;
    return year;
}

std::uint8_t RTC_Object::get_second()
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm* now_tm = std::localtime(&now_time);
    second = now_tm->tm_sec;
    return second;
}

std::uint8_t RTC_Object::get_minute()
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm* now_tm = std::localtime(&now_time);
    minute = now_tm->tm_min;
    return minute;
}
std::uint8_t RTC_Object::get_hour()
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm* now_tm = std::localtime(&now_time);
    hour = now_tm->tm_hour;
    return hour;
}
