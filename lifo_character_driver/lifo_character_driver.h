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

#ifndef __CHR_DUMMY_DRIVER__
#define __CHR_DUMMY_DRIVER__
#include <linux/cdev.h>

// byte size per memory block
#define DRIVER_DEFAULT_BLOCK_SIZE 4
// number of maximum blocks
#define DRIVER_DEFAULT_MAX_BLOCKS 10
// driver name
#define DRIVER_NAME	"lifo_chr_driver"
// driver class
#define DRIVER_CLASS_NAME "lifo_char_driver_class"
// device file name in /dev/
#define DRIVER_DEVICE_FILE_NAME "lifocdev0"
// driver description
#define DRIVER_DESCRIPTION "LIFO CHARACTER MEMORY DRIVER"
// driver author
#define DRIVER_AUTHOR "Kunal Dawn <kunal.dawn@gmail.com>"
// driver license
#define DRIVER_LICENSE "GPL"
// driver version
#define DRIVER_VERSION "0.1"

// structure to store block of memory
typedef struct memblock {
	// data block
	char *data;
	// pointer to next memory block
	struct memblock *next;
} memblock;

// structure to store device information
typedef struct lifo_driver {
	// major number of the device
	int major_number;
	// minor number of the device
	int minor_number;
	// total allocated blocks on this device
	int total_blocks;
	// total data present on the device
	int total_data;
	// linked list header to memory blocks
	memblock *block_header;
	// character device structure
	struct cdev cdev;
	// device class
	struct class *classp;
} lifo_driver;

#endif
