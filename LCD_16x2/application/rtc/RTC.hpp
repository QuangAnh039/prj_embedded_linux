#ifndef RTC_HPP
#define RTC_HPP

#include <cstdint>

class RTC_Object
{
    private:
        std::uint8_t day;
        std::uint8_t month;
        std::uint8_t year;

        std::uint8_t second;
        std::uint8_t minute;
        std::uint8_t hour;
    public:
        RTC_Object(/* args */);
        ~RTC_Object();

        /*Day in year*/
        std::uint8_t get_day();
        std::uint8_t get_month();
        std::uint8_t get_year();

        /*Hour in day*/
        std::uint8_t get_second();
        std::uint8_t get_minute();
        std::uint8_t get_hour();
};

#endif //RTC_HPP