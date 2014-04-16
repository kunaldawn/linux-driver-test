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

static void init_lifo_device_structure(lifo_device *dev) {
	// allocate memory for lifo device structure
	dev = (lifo_device *) kmalloc(sizeof(lifo_device), GFP_KERNEL);
	// allocate memory for memblock header
	dev->block_header = (memblock *) kmalloc(sizeof(memblock), GFP_KERNEL);
	// init device values
	dev->major_number = 0;
	dev->minor_number = 0;
	dev->total_blocks = 0;
	dev->total_data = 0;
	dev->block_header->next = NULL;
}

static void free_dummy_device(lifo_device *dev) {
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
	init_lifo_device_structure(dev);

	// allocate character device region
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
		// unregister character device
		unregister_chrdev_region(first, 1);
		// return error code
		return -1;
	}

	//
	if (device_create(dev->classp, NULL, first, NULL,
	DEVICE_NAME) == NULL) {
		printk(KERN_DEBUG "LIFO DRIVER : INIT FAIL\n");
		class_destroy(dev->classp);
		unregister_chrdev_region(first, 1);
		return -1;
	}
	dev->cdev.owner = THIS_MODULE;
	dev->cdev.ops = &lifo_ops;
	cdev_init(&dev->cdev, &lifo_ops);
	if (cdev_add(&dev->cdev, first, 1) == -1) {
		printk(KERN_DEBUG "LIFO DRIVER : INIT FAIL\n");
		device_destroy(dev->classp, first);
		class_destroy(dev->classp);
		unregister_chrdev_region(first, 1);
		return -1;
	}
	dev->major_number = MAJOR(first);
	dev->minor_number = MINOR(first);
	printk(KERN_DEBUG "LIFO DRIVER : INIT SUCCESS\n");
	return 0;
}

static void __exit lifo_device_exit(void) {
	dev_t first = MKDEV(dev->major_number, dev->minor_number);
	cdev_del(&dev->cdev);
	device_destroy(dev->classp, first);
	class_destroy(dev->classp);
	unregister_chrdev_region(first, 1);
	free_dummy_device(dev);
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
