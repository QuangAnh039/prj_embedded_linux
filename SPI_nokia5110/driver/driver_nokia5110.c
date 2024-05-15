#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/spi/spi.h>
#include <linux/string.h>
#include <linux/of_gpio.h>

#include "nokia5110_ioctl.h"
#include "lcd.h"
dev_t dev = 0; // dev_t hold major n minor number 
static struct class *dev_class;
static struct cdev dev_cdev;
struct nokia_5110 *lcd;


/* Declate the probe and remove functions */
static int nokia5110_probe(struct spi_device *client);
static int nokia5110_remove(struct spi_device *client);
static int nokia5110_open(struct inode *inodep, struct file *device_file); 
static int nokia5110_close(struct inode *inodep, struct file *device_file);
static ssize_t nokia5110_write(struct file *device_file, const char __user *user_buffer, size_t count, loff_t *offs);
static long nokia5110_ioctl(struct file *device_file, unsigned int cmd, unsigned long arg);

static struct file_operations fops = {
    .owner      = THIS_MODULE,
	.open       = nokia5110_open,
    .write      = nokia5110_write,
	.unlocked_ioctl = nokia5110_ioctl,
	.release    = nokia5110_close
};

static int nokia5110_open(struct inode *inodep, struct file *device_file) 
{
	printk("open was called!\n");
	return 0;
}
static int nokia5110_close(struct inode *inodep, struct file *device_file) 
{
	printk("close was called!\n");
	return 0;
}
static ssize_t nokia5110_write(struct file *device_file, const char __user *user_buffer, size_t count, loff_t *offs)
{
	int ret;
	Draw_String_t *str = NULL;

	pr_emerg("%s: lcd-dc: %d\n", __func__, lcd->lcd_dc);
	pr_emerg("%s: lcd-rs: %d\n", __func__, lcd->lcd_rs);

	str = kzalloc(sizeof(Draw_String_t), GFP_KERNEL);
	ret = copy_from_user(str, user_buffer, sizeof(Draw_String_t));
	if (ret) {
		pr_err("can not copy from user\n");
		return -ENOMSG;
	}
	pr_info("\nUser send: %s - address1: %p\n", str->message, str);
	LCD_Puts(str->message, str->pixel, str->font);
	pr_info("\nUser send: %s - address2: %p\n", str->message, lcd);
	LCD_Refresh(lcd);
    printk("write was called!\n");
	
	return count;
}

static long nokia5110_ioctl(struct file *device_file, unsigned int cmd, unsigned long arg)
{
	void __user *argp = (void __user *)arg;
	unsigned char *value = (unsigned char *)argp;
	unsigned char contrast;
	int ret = 0;
	Position_t *pos = NULL;
	Draw_Pixel_t *pixel = NULL;
	Draw_Shape_t *shape = NULL;
	Draw_Circle_t *cir = NULL;

	switch (cmd) {
	case IOCTL_SEND_BUFF:
		LCD_Refresh(lcd);
		break;

	case IOCTL_CLEAR:
		LCD_Clear(lcd);
		break;

	case IOCTL_HOME:
		LCD_Home(lcd);
		break;

	case IOCTL_SET_CONTRAST:
		get_user(contrast, value);
		LCD_SetContrast(lcd, contrast);
		break;

	case IOCTL_GOTOXY:
		pos = kmalloc(sizeof(Position_t), GFP_KERNEL);
		ret = copy_from_user(pos, argp, sizeof(Position_t));
		LCD_GotoXY(lcd, pos->x, pos->y);
		break;

	case IOCTL_DRAW_PIXEL:
		pixel = kmalloc(sizeof(Draw_Pixel_t), GFP_KERNEL);
		ret = copy_from_user(pixel, argp, sizeof(Draw_Pixel_t));
		LCD_DrawPixel(pixel->x, pixel->y, pixel->pixel);
		break;

	case IOCTL_DRAW_LINE:
		shape = kmalloc(sizeof(Draw_Shape_t), GFP_KERNEL);
		ret = copy_from_user(shape, argp, sizeof(Draw_Shape_t));
		LCD_DrawLine(shape->x0, shape->y0, shape->x1, shape->y1, shape->pixel);
		break;

	case IOCTL_DRAW_RECT:
		shape = kmalloc(sizeof(Draw_Shape_t), GFP_KERNEL);
		ret = copy_from_user(shape, argp, sizeof(Draw_Shape_t));
		LCD_DrawRectangle(shape->x0, shape->y0, shape->x1, shape->y1, shape->pixel);
		break;

	case IOCTL_DRAW_FILL_RECT:
		shape = kmalloc(sizeof(Draw_Shape_t), GFP_KERNEL);
		ret = copy_from_user(shape, argp, sizeof(Draw_Shape_t));
		LCD_DrawFilledRectangle(shape->x0, shape->y0, shape->x1, shape->y1, shape->pixel);
		break;

	case IOCTL_DRAW_CIRCLE:
		cir = kmalloc(sizeof(Draw_Circle_t), GFP_KERNEL);
		ret = copy_from_user(cir, argp, sizeof(Draw_Circle_t));
		LCD_DrawCircle(cir->x, cir->y, cir->r, cir->pixel);
		break;

	case IOCTL_DRAW_FILL_CIRCLE:
		cir = kmalloc(sizeof(Draw_Circle_t), GFP_KERNEL);
		ret = copy_from_user(cir, argp, sizeof(Draw_Circle_t));
		LCD_DrawFilledCircle(cir->x, cir->y, cir->r, cir->pixel);
		break;

	default:
		return -ENOTTY;
	}

	return 0;
}
static struct of_device_id nokia5110_driver_ids[] = {
	{
	    .compatible = "nokia5110-LCD",
	}, 
    { /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, nokia5110_driver_ids);

static int nokia5110_probe(struct spi_device *client)
{
	/******************* SPI Device Driver ****************/
	struct device_node *np = client->dev.of_node;
	lcd = kzalloc(sizeof(*lcd), GFP_KERNEL);
	if (lcd == NULL)
	{
		return -1;
	}
	lcd->spi = client;

	lcd->lcd_dc = of_get_named_gpio(np, "lcd-dc", 0);
	lcd->lcd_rs = of_get_named_gpio(np, "lcd-rs", 0);

	spi_set_drvdata(client, lcd);
	pr_emerg("%s: lcd-dc: %d\n", __func__, lcd->lcd_dc);
	pr_emerg("%s: lcd-rs: %d\n", __func__, lcd->lcd_rs);

	printk("LCD Nokia5110 Initializing... \n");
	/*Allocating Major number*/
    if((alloc_chrdev_region(&dev, 0, 1, "nokia5110")) <0) //  appear in /proc/devices/nokia5110
    {
        printk(KERN_INFO "Cannot allocate major number for device 1\n");
        return -1;
    }
	printk(KERN_INFO "Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));
	/*Creating struct class*/
    dev_class = class_create(THIS_MODULE,"lcd_class");   // It will create a structure under/sys/class/lcd_class
    if(IS_ERR(dev_class)){
        pr_err("Cannot create the struct class for device\n");
        goto r_class;
    }
	/*Creating device*/
    if(IS_ERR(device_create(dev_class, &client->dev, dev, lcd,"nokia5110"))) // create /dev/nokia5110
    {
        pr_err("Cannot create the Device\n");
        goto r_device;
	}
	/*Creating cdev structure*/
	cdev_init(&dev_cdev, &fops);
	/*Adding character device to the system*/
	if((cdev_add(&dev_cdev,dev,1)) < 0){
        pr_info("Cannot add the device to the system\n");
        goto rm_device;
    }
	/*************** Management LCD devices *************/
	LCD_Init(lcd, 0x38);
	printk(KERN_INFO "Successfully Initialize...\n");
	return 0;
rm_device: 
    device_destroy(dev_class,dev);
r_device:
    class_destroy(dev_class);
r_class:
    unregister_chrdev_region(dev,1);
    return -1;
};

static int nokia5110_remove(struct spi_device *client)
{
	cdev_del(&dev_cdev);  
	device_destroy(dev_class,dev);
	class_destroy(dev_class);
	unregister_chrdev_region(dev, 1);
	LCD_Clear(lcd);
	LCD_free_IO(lcd);
	kfree(lcd); 
	printk("LCD Nokia5110 shutdown... \n");
	return 0;
};
static struct spi_driver nokia5110_spi_driver = {
	.probe	= nokia5110_probe,
	.remove	= nokia5110_remove,
	.driver = {
		.name = "nokia5110_spi", /* /sys/bus/spi/drivers/.... */
		.owner = THIS_MODULE,
		.of_match_table = nokia5110_driver_ids,
	},
};
module_spi_driver(nokia5110_spi_driver);


MODULE_AUTHOR("Nguyen Quang Anh");
MODULE_DESCRIPTION("A driver for nokia5110 LCD");
MODULE_LICENSE("GPL");