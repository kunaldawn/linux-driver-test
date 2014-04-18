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
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/semaphore.h>
#include <uapi/asm-generic/errno-base.h>
#include "lifo_character_driver.h"
#include "memory_manager.h"

// lifo device structure
static lifo_driver *dev;

static int lifo_open(struct inode *i, struct file *f) {
	lifo_driver *device;
	// print debug info
	printk(KERN_DEBUG "LIFO DRIVER : OPEN CALL\n");
	// get the device pointer
	device = container_of(i->i_cdev, lifo_driver, cdev);
	// store the device pointer in private data
	f->private_data = device;
	// return success code
	return 0;
}

static int lifo_close(struct inode *i, struct file *f) {
	// print debug info
	printk(KERN_DEBUG "LIFO DRIVER : CLOSE CALL\n");
	// return success code
	return 0;
}

static ssize_t lifo_read(struct file *f, char __user *buff, size_t len,
		loff_t *off) {
	// temp variable
	int data_available_on_block = 0, data_to_read = 0, data_read_counter = 0;
	// temporary variable
	lifo_driver *device;
	// print debug info
	char *redable_buffer;
	printk(KERN_DEBUG "LIFO DRIVER : READ CALL\n");
	// get the device pointer
	device = f->private_data;

	// mutex lock
	if (down_interruptible(&device->sem)) {
		// return error code for interrupted
		return -ERESTARTSYS;
	}

	// loop for ever
	while (1) {
		// check if any data is available
		if (device->total_data == 0) {
			printk(KERN_DEBUG "LIFO DRIVER : NO DATA\n");

			// unlock mutex
			up(&device->sem);

			// return EOF
			return 0;
		}

		// calculate available data in last block
		data_available_on_block = (device->total_data
				- ((device->total_blocks - 1) * DRIVER_DEFAULT_BLOCK_SIZE));

		// calculate data to read from this block
		data_to_read =
				(data_available_on_block < len) ? data_available_on_block : len;

		// check if any data is available in last block
		if (data_available_on_block == 0) {
			// free last block because no data available in this block
			free_last_block(device->block_header);
			// reduce block counter
			device->total_blocks--;
			// print debug info
			printk(KERN_DEBUG "LIFO DRIVER : READ [REMOVE BLOCK][BLOCKS:%d]\n",
					device->total_blocks);
		} else {
			// get readable buffer from last block
			redable_buffer = get_readable_buffer(device->block_header,
					data_available_on_block, data_to_read);
			data_read_counter = data_to_read;

			// copy data in reverse order
			while (data_read_counter >= 0) {
				if (copy_to_user(buff, redable_buffer + data_read_counter, 1)
						!= 0) {
					// unlock mutex
					up(&device->sem);

					// return error code
					return -EFAULT;
				}
				// decrement read counter
				data_read_counter--;
			}

			// decrement data counter
			device->total_data -= data_to_read;

			// print debug info
			printk(KERN_DEBUG "LIFO DRIVER : READ SUCCESS [SIZE:%d/%d]\n",
					data_to_read, len);

			// unlock mutex
			up(&device->sem);

			// return amount of data read
			return data_to_read;
		}
	}

	// unlock mutex
	up(&device->sem);

	// return error
	return -EFAULT;
}

static ssize_t lifo_write(struct file *f, const char __user *buff, size_t len,
		loff_t *off) {
// temporary variables
	int freespace_inblock = 0, data_to_write = 0, data_not_copied = 0;
// pointer to writable buffer starting position
	char *writable_buffer;
// get the device pointer
	lifo_driver *device = f->private_data;
// print debug info
	printk(
	KERN_DEBUG "LIFO DRIVER : WRITE CALL");

// mutex lock
	if (down_interruptible(&device->sem)) {
		// return error code for interrupted
		return -ERESTARTSYS;
	}

// calculate free space in last block, if any
	freespace_inblock = ((device->total_blocks * DRIVER_DEFAULT_BLOCK_SIZE)
			- device->total_data);

// check if there is any free space
	if (freespace_inblock > 0) {
		// get the writable buffer from the block
		writable_buffer = get_writable_buffer(device->block_header,
		DRIVER_DEFAULT_BLOCK_SIZE - freespace_inblock);

		// calculate how much data we can write in this call
		data_to_write = (len <= freespace_inblock) ? len : freespace_inblock;

		// copy data from user space to kernel space
		data_not_copied = copy_from_user(writable_buffer, buff, data_to_write);

		// update total data counter
		device->total_data += data_to_write - data_not_copied;

		// print debug info
		printk(
				KERN_DEBUG "LIFO DRIVER : WRITE SUCCESS [BLOCKS:%d][SIZE:%d][WRITE:%d/%d]\n",
				device->total_blocks, device->total_data, len,
				data_to_write - data_not_copied);

		// unlock mutex
		up(&device->sem);

		// return byte count that have been successfully copied
		return data_to_write - data_not_copied;
	}

// there is no free space in the last block, or no block at all.
// check if new block allocation is possible
	if (device->total_blocks >= DRIVER_DEFAULT_MAX_BLOCKS) {
		// print debug info
		printk(
		KERN_DEBUG "LIFO DRIVER : WRITE FAIL : MAX_BLOCKS\n");

		// unlock mutex
		up(&device->sem);

		// return error code
		return -EFAULT;
	}

// check memory allocation was success
	if (allocate_new_block(device->block_header) == 1) {
		// print debug info
		printk(
		KERN_DEBUG "LIFO DRIVER : WRITE FAIL : MEMORY_ALLOCATION\n");

		// unlock mutex
		up(&device->sem);

		// return error code
		return -EFAULT;
	}

// increment available blocks count
	device->total_blocks++;

// get the pointer to writable buffer
	writable_buffer = get_writable_buffer(device->block_header, 0);

// calculate how much data to write
	data_to_write =
			(len <= DRIVER_DEFAULT_BLOCK_SIZE) ?
					len : DRIVER_DEFAULT_BLOCK_SIZE;

// copy data from user space to kernel space
	data_not_copied = copy_from_user(writable_buffer, buff, data_to_write);

// update total data counter
	device->total_data += data_to_write - data_not_copied;

// print debug info
	printk(
			KERN_DEBUG "LIFO DRIVER : WRITE SUCCESS[NEW BLOCK] WRITE SUCCESS [BLOCKS:%d][SIZE:%d][WRITE:%d/%d]\n",
			device->total_blocks, device->total_data, len,
			data_to_write - data_not_copied);

// unlock mutex
	up(&device->sem);

// return how much data actually copied
	return data_to_write - data_not_copied;
}

// define file operations
static struct file_operations lifo_ops =
		{ .owner = THIS_MODULE, .open = lifo_open, .release = lifo_close,
				.read = lifo_read, .write = lifo_write, };

static int init_lifo_device_structure(void) {
// allocate memory for lifo device structure
	dev = kmalloc(sizeof(lifo_driver), GFP_KERNEL);

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
// free all allocated blocks
	free_all_blocks(dev->block_header);
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
	if (alloc_chrdev_region(&first, 0, 1, DRIVER_NAME) < 0) {
		// print debug msg
		printk(KERN_DEBUG "LIFO DRIVER : INIT FAIL(ALLOC CHR DEV)\n");
		// return error code
		return -1;
	}

// create device class
	if ((dev->classp = class_create(THIS_MODULE, DRIVER_CLASS_NAME)) == NULL) {
		// print debug info
		printk(KERN_DEBUG "LIFO DRIVER : INIT FAIL(CREATE_DEV_CLASS)\n");
		// unregister device region
		unregister_chrdev_region(first, 1);
		// return error code
		return -1;
	}

// create device and register with sysfs
	if (device_create(dev->classp, NULL, first, NULL,
	DRIVER_DEVICE_FILE_NAME) == NULL) {
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

// init semaphore
	sema_init(&dev->sem, 1);

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
MODULE_LICENSE(DRIVER_LICENSE);
// set module author
MODULE_AUTHOR(DRIVER_AUTHOR);
// set module description
MODULE_DESCRIPTION(DRIVER_DESCRIPTION);
// set module version
MODULE_VERSION(DRIVER_VERSION);
