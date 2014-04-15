#include<linux/module.h>
#include<linux/version.h>
#include<linux/kernel.h>
#include<linux/types.h>
#include<linux/fs.h>
#include<linux/kdev_t.h>
#include<linux/device.h>
#include<linux/cdev.h>
#include<asm/uaccess.h>

static dev_t dev; // first device number
static struct cdev c_dev; // char device structure
static struct class *d_class; // device class
static char data;
static int data_available;

static int dummy_open(struct inode *i, struct file *f)
{
	printk(KERN_DEBUG "DUMMY DRIVER : OPEN CALL\n");
	return 0;
}

static int dummy_close(struct inode *i, struct file *f)
{
	printk(KERN_DEBUG "DUMMY DRIVER : CLOSE CALL\n");
	return 0;
}

static ssize_t dummy_read(struct file *f, char __user *buff, size_t len, loff_t *off)
{
	printk(KERN_DEBUG "DUMMY DRIVER : READ CALL\n");
	if(data_available > 0)
	{
		if(copy_to_user(buff,&data,1) == 0)
		{
			data_available = 0;
			return 1;
		}else
		{
			return EFAULT;
		}
	}
	return 0;
}

static ssize_t dummy_write(struct file *f, const char __user *buff, size_t len, loff_t *off)
{
	printk(KERN_DEBUG "DUMMY DRIVER : WRITE CALL\n");
	if(data_available == 0)
	{
		if(copy_from_user(&data ,buff, 1) == 0)
		{
			printk(KERN_DEBUG "DATA WRITTEN TO DEVICE %c\n",data);
			data_available = 1;
			return len;
		}else
		{
			return EFAULT;
		}
	}
	return len;
}

static struct file_operations dummy_ops = 
{
	.owner = THIS_MODULE,
	.open = dummy_open,
	.release = dummy_close,
	.read = dummy_read,
	.write = dummy_write
};

static int __init dummy_init(void)
{
	printk(KERN_EMERG "DUMMY DRIVER : LOADED\n");
	data_available = 0;
	if(alloc_chrdev_region(&dev, 0, 1, "dummy") < 0)
	{
		printk(KERN_DEBUG "DUMMY DRIVER : UNABLE TO CREATE CHAR DEVICE\n");
		return -1;
	}
	if((d_class = class_create(THIS_MODULE, "dummy_device")) == NULL)
	{
		unregister_chrdev_region(dev,1);
		return -1;
	}
	if(device_create(d_class,NULL,dev,NULL,"dummy") == NULL)
	{
		class_destroy(d_class);
		unregister_chrdev_region(dev,1);
		return -1;
	}
	cdev_init(&c_dev, &dummy_ops);
	if(cdev_add(&c_dev,dev,1) == -1)
	{
		device_destroy(d_class,dev);
		class_destroy(d_class);
		unregister_chrdev_region(dev,1);
		return -1;
	}
	printk(KERN_DEBUG "DUMMY DRIVER : SUCCESS CREATING CHAR DEVICE\n");
	return 0;
}

static void __exit dummy_exit(void)
{
	cdev_del(&c_dev);
	device_destroy(d_class,dev);
	class_destroy(d_class);
	unregister_chrdev_region(dev, 1);
	printk(KERN_EMERG "DUMMY DRIVER : UNLOADED\n");
}

module_init(dummy_init);
module_exit(dummy_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kunal Dawn <kunal.dawn@gmail.com>");
MODULE_DESCRIPTION("Dummy Driver");
