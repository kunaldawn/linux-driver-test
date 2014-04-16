//------------------------------------------------------------------------------
//dummy-driver devl base
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
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <uapi/asm-generic/errno-base.h>
#include "lifo_driver.h"

// lifo device structure
static lifo_device *dev;

static int lifo_open(struct inode *i, struct file *f) {
	printk(KERN_DEBUG "LIFO DRIVER : OPEN CALL\n");
	return 0;
}

static int lifo_close(struct inode *i, struct file *f) {
	printk(KERN_DEBUG "LIFO DRIVER : CLOSE CALL\n");
	return 0;
}

static ssize_t lifo_read(struct file *f, char __user *buff, size_t len,
		loff_t *off) {
	printk(KERN_DEBUG "LOFO DRIVER : READ CALL\n");
	return 0;
}

static ssize_t lifo_write(struct file *f, const char __user *buff, size_t len,
		loff_t *off) {
	printk(KERN_DEBUG "LIFO DRIVER : WRITE CALL\n");
	return EFAULT;
}

// define file operations
static struct file_operations lifo_ops =
		{ .owner = THIS_MODULE, .open = lifo_open, .release = lifo_close,
				.read = lifo_read, .write = lifo_write, };

static int init_lifo_device_structure(void) {
	// allocate memory for lifo device structure
	dev = kmalloc(sizeof(lifo_device), GFP_KERNEL);

	// check if allocation was successful
	if (dev == NULL) {
		printk(KERN_DEBUG "LIFO DRIVER : ALLOC FAIL 1\n");
		//return error code
		return 0;
	}

	// allocate memory for memblock header
	dev->block_header = kmalloc(sizeof(memblock), GFP_KERNEL);
	// check if allocation was successful
	if (dev->block_header == NULL) {
		//return error code
		kfree(dev);
		return 0;
	}

	// init major number
	dev->major_number = 0;
	// init minor number
	dev->minor_number = 0;
	// init total allocated memory blocks
	dev->total_blocks = 0;
	// init total data used
	dev->total_data = 0;
	// init memory block header
	dev->block_header->next = NULL;
	// return success code
	return 1;
}

static void free_dummy_device(void) {
	// free allocated memory for block header
	kfree(dev->block_header);
	// free allocated memory for device info
	kfree(dev);
}

static int __init lifo_device_init(void) {
	// var to hold first major/minor number
	dev_t first;
	// print debug message
	printk(KERN_EMERG "LIFO DRIVER : LOADING\n");

	// allocate and init lifo device info
	if (init_lifo_device_structure() == 0) {
		// print debug msg
		printk(KERN_DEBUG "LIFO DRIVER : INIT FAIL(MEMORY ALLOCATION)\n");
		// return error code
		return -1;
	}

	// allocate first major and minor number and register device region
	if (alloc_chrdev_region(&first, 0, 1, DEVICE_NAME) < 0) {
		// print debug msg
		printk(KERN_DEBUG "LIFO DRIVER : INIT FAIL(ALLOC CHR DEV)\n");
		// return error code
		return -1;
	}

	// create device class
	if ((dev->classp = class_create(THIS_MODULE, DEVICE_CLASS_NAME)) == NULL) {
		// print debug info
		printk(KERN_DEBUG "LIFO DRIVER : INIT FAIL(CREATE_DEV_CLASS)\n");
		// unregister device region
		unregister_chrdev_region(first, 1);
		// return error code
		return -1;
	}

	// create device and register with sysfs
	if (device_create(dev->classp, NULL, first, NULL,
	DEVICE_NAME) == NULL) {
		// print debug info
		printk(KERN_DEBUG "LIFO DRIVER : INIT FAIL\n");
		// destroy device class
		class_destroy(dev->classp);
		// unregister device region
		unregister_chrdev_region(first, 1);
		// return error code
		return -1;
	}

	// set owner of cdev
	dev->cdev.owner = THIS_MODULE;
	// set file operations of cdev
	dev->cdev.ops = &lifo_ops;
	// init cdev
	cdev_init(&dev->cdev, &lifo_ops);

	// add the device to the system
	if (cdev_add(&dev->cdev, first, 1) == -1) {
		// print debug info
		printk(KERN_DEBUG "LIFO DRIVER : INIT FAIL\n");
		// destroy the device info from sysfs
		device_destroy(dev->classp, first);
		// destroy device class
		class_destroy(dev->classp);
		// unregister device region
		unregister_chrdev_region(first, 1);
		// return error code
		return -1;
	}

	// save the allocated major number
	dev->major_number = MAJOR(first);
	// save the allocated minor number
	dev->minor_number = MINOR(first);
	// print debug info
	printk(KERN_DEBUG "LIFO DRIVER : INIT SUCCESS\n");
	// return success code
	return 0;
}

static void __exit lifo_device_exit(void) {
	// get the first device major/minor
	dev_t first = MKDEV(dev->major_number, dev->minor_number);
	// delete character device
	cdev_del(&dev->cdev);
	// destroy device
	device_destroy(dev->classp, first);
	// destroy device class
	class_destroy(dev->classp);
	// unregister device region
	unregister_chrdev_region(first, 1);
	// free allocated memory
	free_dummy_device();
	// print debug message
	printk(KERN_EMERG "LIFO DRIVER : UNLOADED\n");
}

// load module
module_init(lifo_device_init);
// unload module
module_exit(lifo_device_exit);

// set module license
MODULE_LICENSE(DEVICE_LICENSE);
// set module author
MODULE_AUTHOR(DEVICE_AUTHOR);
// set module description
MODULE_DESCRIPTION(DEVICE_DES);
// set module version
MODULE_VERSION(DEVICE_VERSION);
