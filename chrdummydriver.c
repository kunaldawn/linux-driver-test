#include<linux/module.h>
#include<linux/version.h>
#include<linux/kernel.h>
#include<linux/types.h>
#include<linux/fs.h>
#include<linux/kdev_t.h>
#include<linux/device.h>
#include<linux/cdev.h>
#include<linux/slab.h>
#include<asm/uaccess.h>

static dev_t dev; // first device number
static struct cdev c_dev; // char device structure
static struct class *d_class; // device class
static int data_available;
static int block_size;
static char *data;

static int dummychr_open(struct inode *i, struct file *f)
{
	printk(KERN_DEBUG "DUMMY CHR DRIVER : OPEN CALL\n");
	return 0;
}

static int dummychr_close(struct inode *i, struct file *f)
{
	printk(KERN_DEBUG "DUMMY CHR DRIVER : CLOSE CALL\n");
	return 0;
}

static ssize_t dummychr_read(struct file *f, char __user *buff, size_t len, loff_t *off)
{
	printk(KERN_DEBUG "DUMMY CHR DRIVER : READ CALL\n");
	printk(KERN_DEBUG "READ_OFFSET = %d\n",(int)*off);
	if(*off < data_available)
	{

		if(copy_to_user(buff,data + (long)*off,1) == 0)
		{
			*off += 1;
			return 1;
		}else
		{
			return EFAULT;
		}
	}
	return 0;
}

static ssize_t dummychr_write(struct file *f, const char __user *buff, size_t len, loff_t *off)
{
	printk(KERN_DEBUG "DUMMY CHR DRIVER : WRITE CALL\n");
	if(data_available < block_size)
	{
		if(copy_from_user(data + data_available, buff, 1) == 0)
		{
			data_available = data_available + 1;
			*off += 1;
			printk(KERN_DEBUG "DATA_AVAILABLE = %d\n",data_available);
			return 1;
		}else
		{
			return EFAULT;
		}
	}
	return EFAULT;
}

static struct file_operations dummychr_ops = 
{
	.owner = THIS_MODULE,
	.open = dummychr_open,
	.release = dummychr_close,
	.read = dummychr_read,
	.write = dummychr_write,
};

static int __init dummychr_init(void)
{
	printk(KERN_EMERG "DUMMY CHRCHR DRIVER : LOADING\n");
	data_available = 0;
	if(alloc_chrdev_region(&dev, 0, 1, "dummychr") < 0)
	{
		printk(KERN_DEBUG "DUMMY CHR DRIVER : UNABLE TO CREATE CHAR DEVICE\n");
		return -1;
	}
	if((d_class = class_create(THIS_MODULE, "dummychr_device")) == NULL)
	{
		unregister_chrdev_region(dev,1);
		return -1;
	}
	if(device_create(d_class,NULL,dev,NULL,"dummychr") == NULL)
	{
		class_destroy(d_class);
		unregister_chrdev_region(dev,1);
		return -1;
	}
	c_dev.owner = THIS_MODULE;
	c_dev.ops = &dummychr_ops;
	cdev_init(&c_dev, &dummychr_ops);
	if(cdev_add(&c_dev,dev,1) == -1)
	{
		device_destroy(d_class,dev);
		class_destroy(d_class);
		unregister_chrdev_region(dev,1);
		return -1;
	}
	data_available = 0;
	block_size = 100000;
	data = (char *)kmalloc(sizeof(char) * block_size, GFP_KERNEL);
	printk(KERN_DEBUG "DUMMY CHR DRIVER : SUCCESS CREATING CHAR DEVICE\n");
	return 0;
}

static void __exit dummychr_exit(void)
{
	kfree(data);
	cdev_del(&c_dev);
	device_destroy(d_class,dev);
	class_destroy(d_class);
	unregister_chrdev_region(dev, 1);
	printk(KERN_EMERG "DUMMY CHR DRIVER : UNLOADED\n");
}

module_init(dummychr_init);
module_exit(dummychr_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kunal Dawn <kunal.dawn@gmail.com>");
MODULE_DESCRIPTION("Dummy CHR Driver");
