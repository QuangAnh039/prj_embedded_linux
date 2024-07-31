#ifndef __LCD_HPP__
#define __LCD_HPP__

#include "../../driver_lcd/lcd_16x2_ioctl.h"
#include <string>

#define FILENAME 	"/dev/lcd16x2"

class LCD_Object 
{
    public:
        void print_string(std::string string);
        void set_xy();
        void clear_screen();
};


#endif 