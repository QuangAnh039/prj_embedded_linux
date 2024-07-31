
BBB_KERNEL := /home/quanganh/kernelbuildscripts/KERNEL
TOOLCHAIN := /home/quanganh/kernelbuildscripts/dl/gcc-8.5.0-nolibc/arm-linux-gnueabi/bin/arm-linux-gnueabi-

EXTRA_CFLAGS=-Wall
obj-m := lcd_16x2.o
CFLAGS_MODULE += -fstack-protector

lcd_16x2-objs := lcd.o driver_lcd.o

all:
	make ARCH=arm CROSS_COMPILE=$(TOOLCHAIN) -C $(BBB_KERNEL) M=$(shell pwd) modules
clean:
	make -C $(BBB_KERNEL) M=$(shell pwd) clean