//------------------------------------------------------------------------------
//linux-device-driver
//Copyright (C) 2014  Kunal Dawn <kunal.dawn@gmail.com>
//
//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program.  If not, see <http://www.gnu.org/licenses/>.
//------------------------------------------------------------------------------

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
#include <uapi/asm-generic/errno-base.h>

// driver name
#define DRIVER_NAME "null_char_driver"
// driver class name
#define DRIVER_CLASS_NAME "null_char_driver_class"
// driver version
#define DRIVER_VERSION "0.1"
// driver description
#define DRIVER_DESCRIPTION "NULL CHARACTER DEVICE"
// driver author
#define DRIVER_AUTHOR "Kunal Dawn <kunal.dawn@gmail.com>"
// driver license
#define DRIVER_LICENCE "GPL"
// device file name in /dev/
#define DRIVER_DEVICE_FILE_NAME "nullcdev0"

// first device major/minor
static dev_t dev_num;
// cdev structure
static struct cdev cdev;
// device class structure
static struct class *classp;

static int null_open(struct inode *i, struct file *f) {
	// print debug info
	printk(KERN_DEBUG "NULL DRIVER : OPEN CALL\n");
	// return success
	return 0;
}

static int null_close(struct inode *i, struct file *f) {
	// print debug info
	printk(KERN_DEBUG "NULL DRIVER : CLOSE CALL\n");
	// return success
	return 0;
}

static ssize_t null_read(struct file *f, char __user *buff, size_t len,
		loff_t *off) {
	// print debug info
	printk(KERN_DEBUG "NULL DRIVER : READ CALL\n");
	// return no data available
	return 0;
}

static ssize_t null_write(struct file *f, const char __user *buff, size_t len,
		loff_t *off) {
	// print debug info
	printk(KERN_DEBUG "NULL DRIVER : WRITE CALL\n");
	// return all data is written
	return len;
}

static struct file_operations dummy_ops =
		{ .owner = THIS_MODULE, .open = null_open, .release = null_close,
				.read = null_read, .write = null_write };

static int __init dummy_init(void) {
	// print debug info
	printk(KERN_EMERG "NULL DRIVER : INIT REQUEST\n");

	// check character device region allocation
	if (alloc_chrdev_region(&dev_num, 0, 1, DRIVER_NAME) < 0) {
		// print debug info
		printk(KERN_DEBUG "NULL DRIVER : UNABLE TO REGISTER DEVICE REGION\n");
		// return error code
		return -1;
	}

	// check device class creation
	if ((classp = class_create(THIS_MODULE, DRIVER_CLASS_NAME)) == NULL) {
		// print debug info
		printk(KERN_DEBUG "NULL DRIVER : UNABLE TO CREATE DEVICE CLASS\n");
		// unregister character device region
		unregister_chrdev_region(dev_num, 1);
		// return error code
		return -1;
	}

	// check device creation
	if (device_create(classp, NULL, dev_num, NULL, DRIVER_DEVICE_FILE_NAME) == NULL) {
		// print debug info
		printk(KERN_DEBUG "NULL DRIVER : UNABLE TO CREATE DEVICE\n");
		// destroy device class
		class_destroy(classp);
		// unregister character device region
		unregister_chrdev_region(dev_num, 1);
		// return error code
		return -1;
	}

	// init cdev structure
	cdev_init(&cdev, &dummy_ops);
	// add device to system
	if (cdev_add(&cdev, dev_num, 1) == -1) {
		// print debug message
		printk(KERN_DEBUG "NULL DRIVER : UNABLE TO ADD DEVICE TO SYSTEM\n");
		// destroy device
		device_destroy(classp, dev_num);
		// destroy device class
		class_destroy(classp);
		// unregister character device region
		unregister_chrdev_region(dev_num, 1);
		// return error code
		return -1;
	}

	// print debug info
	printk(KERN_DEBUG "NULL DRIVER : INIT SUCCESS\n");
	// return success code
	return 0;
}

static void __exit dummy_exit(void) {
	// delete character device
	cdev_del(&cdev);
	// destroy device
	device_destroy(classp, dev_num);
	// destroy device class
	class_destroy(classp);
	// unregister character device region
	unregister_chrdev_region(dev_num, 1);
	// print debug message
	printk(KERN_EMERG "NULL DRIVER : UNLOADED\n");
}

// init module
module_init(dummy_init);
// unload module
module_exit(dummy_exit);

// set module license
MODULE_LICENSE(DRIVER_LICENCE);
// set module author
MODULE_AUTHOR(DRIVER_AUTHOR);
// set module description
MODULE_DESCRIPTION(DRIVER_DESCRIPTION);
// set module version
MODULE_VERSION(DRIVER_VERSION);
