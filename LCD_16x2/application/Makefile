CC = arm-linux-gnueabihf-g++
CFLAGS = -Wall -Wextra -O2

# Mục tiêu chính
main: main.o lcd/LCD.o lcd/lcd_i2c/LCD_I2C.o rtc/RTC.o
	$(CC) -o main main.o lcd/LCD.o lcd/lcd_i2c/LCD_I2C.o rtc/RTC.o

# Biên dịch main.cpp thành main.o
main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp -o main.o

# Biên dịch lcd/LCD.cpp thành lcd/LCD.o
lcd/LCD.o: lcd/LCD.cpp lcd/LCD.hpp
	$(CC) $(CFLAGS) -c lcd/LCD.cpp -o lcd/LCD.o

# Biên dịch lcd/lcd_i2c/LCD_I2C.cpp thành lcd/lcd_i2c/LCD_I2C.o
lcd/lcd_i2c/LCD_I2C.o: lcd/lcd_i2c/LCD_I2C.cpp lcd/lcd_i2c/LCD_I2C.hpp
	$(CC) $(CFLAGS) -c lcd/lcd_i2c/LCD_I2C.cpp -o lcd/lcd_i2c/LCD_I2C.o

# Biên dịch rtc/RTC.cpp thành rtc/RTC.o
rtc/RTC.o: rtc/RTC.cpp rtc/RTC.hpp
	$(CC) $(CFLAGS) -c rtc/RTC.cpp -o rtc/RTC.o

# Xóa các tệp đối tượng và tệp thực thi
clean:
	rm -f main.o lcd/LCD.o lcd/lcd_i2c/LCD_I2C.o rtc/RTC.o main
