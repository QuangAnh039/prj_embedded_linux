#include <iostream>
#include "./rtc/RTC.hpp"
#include "./lcd/lcd_i2c/LCD_I2C.hpp"
#include <string>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
using namespace std;

int main()
{
    RTC_Object clock;
    LCD_I2C lcd16x2;
    while(1)
    {
        lcd16x2.print_string((to_string(clock.get_day()) + "/" + to_string(clock.get_month()) + "/" + to_string(clock.get_year())));
        lcd16x2.set_xy();
        lcd16x2.print_string((to_string(clock.get_hour()) + ":" + to_string(clock.get_minute()) + "/" + to_string(clock.get_second())));
        sleep(1);
        lcd16x2.clear_screen();
    }

}