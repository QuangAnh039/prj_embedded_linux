#include "LCD.hpp"
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <iostream>

static int open_file()
{
	int fd;

	fd = open(FILENAME, O_RDWR);
	if (fd < 0) 
	{
		perror("open: Failed\n");
		return fd;
	}

	return fd;
}
void LCD_Object::print_string(std::string mes)
{
    int fd = open_file();
	if(write(fd, mes.c_str(), mes.size()) < 0)
	{
		std::cout << "Write: Failed";
		return;
	}
	close(fd);
}

void LCD_Object::set_xy()
{
	int fd = open_file();
	if (ioctl(fd, IOCTL_SETXY) < 0) 
	{
		printf("ioctl: Failed at %s\n", __func__);
		return;
	}
	close(fd);
}

void LCD_Object::clear_screen()
{
	int fd = open_file();
	if (ioctl(fd, IOCTL_CLEAR) < 0) 
	{
		printf("ioctl: Failed at %s\n", __func__);
		return;
	}
	close(fd);
}