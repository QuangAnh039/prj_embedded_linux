#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/of_device.h>
#include <linux/i2c.h>
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
#include <linux/string.h>
#include <linux/of_gpio.h>

#include "lcd_16x2_ioctl.h"
#include "lcd.h"

dev_t dev = 0; // dev_t hold major n minor number 
static struct class *dev_class;
static struct cdev dev_cdev;
struct lcd_16x2 *lcd = NULL;
/* Declare the probe and remove functions */
static int lcd_16x2_probe_new(struct i2c_client *client);
static int lcd_16x2_remove(struct i2c_client *client);
static int lcd_16x2_open(struct inode *inodep, struct file *device_file); 
static int lcd_16x2_close(struct inode *inodep, struct file *device_file);
static ssize_t lcd_16x2_write(struct file *device_file, const char __user *user_buffer, size_t len, loff_t *offs);
static long lcd_16x2_ioctl(struct file *device_file, unsigned int cmd, unsigned long arg);

static struct file_operations fops = {
    .owner      = THIS_MODULE,
	.open       = lcd_16x2_open,
    .write      = lcd_16x2_write,
	.unlocked_ioctl = lcd_16x2_ioctl,
	.release    = lcd_16x2_close
};

static int lcd_16x2_open(struct inode *inodep, struct file *device_file) 
{
	printk("open was called!\n");
	return 0;
}

static int lcd_16x2_close(struct inode *inodep, struct file *device_file) 
{
	printk("close was called!\n");
	return 0;
}

static ssize_t lcd_16x2_write(struct file *device_file, const char __user *user_buffer, size_t count, loff_t *offs)
{
    int ret;
    char *str = kmalloc(count, GFP_KERNEL); // Cấp phát đủ 'count' byte
    if (!str)
    {
        return -ENOMEM;
    }

    ret = copy_from_user(str, user_buffer, count);
    if (ret)
    {
        pr_err("Cannot copy from user, %d bytes not copied\n", ret);
        kfree(str); // Giải phóng bộ nhớ khi có lỗi
        return -EFAULT;
    }

    str[count - ret] = '\0'; // Đảm bảo chuỗi có kết thúc null
    lcd_send_str(lcd->client, str);
    //msleep(1);
    kfree(str);
    return count - ret; // Trả về số byte đã ghi thành công
}

static long lcd_16x2_ioctl(struct file *device_file, unsigned int cmd, unsigned long arg)
{
    switch (cmd) {
    case IOCTL_CLEAR:
        lcd_clean(lcd->client);
        break;
    case IOCTL_SETXY:
        LCD_Goto_XY(lcd->client, 2, 1);
        break;
    default:
        return -ENOTTY;    
    }
    return 0;
}

static struct of_device_id lcd_16x2_of_id[] = {
	{.compatible = "lcd_16x2", }, 
    { /* sentinel */ }
};

MODULE_DEVICE_TABLE(of, lcd_16x2_of_id);

static int lcd_16x2_probe_new(struct i2c_client *client)
{
    lcd = (struct lcd_16x2 *)kzalloc(sizeof(struct lcd_16x2), GFP_KERNEL);
	if (lcd == NULL)
	{
		return -1;
	}
    lcd->client = client;
    printk("LCD LCD_16x2 Initializing... \n");
    printk("ver2... \n");
    /*Allocating Major number*/
    if((alloc_chrdev_region(&dev, 0, 1, "lcd16x2")) <0) //  appear in /proc/devices/lcd16x2
    {
        printk(KERN_INFO "Cannot allocate major number for device 1\n");
        return -1;
    }
    printk(KERN_INFO "Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));
    /*Creating struct class*/
    dev_class = class_create(THIS_MODULE,"lcd_class");   // It will create a structure under/sys/class/lcd_class
    if(IS_ERR(dev_class))
    {
        pr_err("Cannot create the struct class for device\n");
        goto r_class;
    }
    /*Creating device*/
    if(IS_ERR(device_create(dev_class, &client->dev, dev, lcd,"lcd16x2"))) // create /dev/lcd16x2
    {
        pr_err("Cannot create the Device\n");
        goto r_device;
	}
    /*Creating cdev structure*/
	cdev_init(&dev_cdev, &fops);
    /*Adding character device to the system*/
	if((cdev_add(&dev_cdev,dev,1)) < 0)
    {
        pr_info("Cannot add the device to the system\n");
        goto rm_device;
    }
    lcd_init(client);
    
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

static int lcd_16x2_remove(struct i2c_client *client)
{   
    cdev_del(&dev_cdev);  
	device_destroy(dev_class,dev);
	class_destroy(dev_class);
	unregister_chrdev_region(dev, 1);
    lcd_clean(client);
    kfree(lcd);

    printk("LCD16x2 shutdown... \n");
    return 0;
};

static struct i2c_driver lcd_16x2_driver = {
	.probe_new	= lcd_16x2_probe_new,
	.remove		= lcd_16x2_remove,
	.driver		= {
		.name = "lcd_16x2_driver",
        .owner = THIS_MODULE,
		.of_match_table = lcd_16x2_of_id,
	},
};

module_i2c_driver(lcd_16x2_driver);

MODULE_AUTHOR("Nguyen Quang Anh");
MODULE_DESCRIPTION("A driver for LCD_16x2");
MODULE_LICENSE("GPL");
