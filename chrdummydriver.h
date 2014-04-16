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

#ifndef __CHR_DUMMY_DRIVER__
#define __CHR_DUMMY_DRIVER__

#define DEFAULT_BLOCK_SIZE 1024
#define DEFAULT_MAX_BLOCKS 100

typedef struct memblock {
	char *data;
	struct memblock *next;
}memblock;

typedef struct dummychr_device
{
	int major_number;
	int minor_number;
	int total_blocks;
	int total_data;
	memblock *data_block;
	struct cdev *chr_device;
	struct class *chr_device_class;
}dummychr_device;

#endif
