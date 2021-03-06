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

#ifndef __MEM_MANAGER__
#define __MEM_MANAGER__

#include "lifo_character_driver.h"

// get last memory block available
memblock * get_last_block(memblock *head);
// allocate new memory block at the end
int allocate_new_block(memblock * head);
// free all space reserved by all blocks
void free_all_blocks(memblock *head);
// get pointer to writable space in blocks data buffer
char* get_writable_buffer(memblock *head, int data_size);
// get pointer to readable space in blocks data buffer
char* get_readable_buffer(memblock *head, int data_size, int len);
// free last allocated block
void free_last_block(memblock *head);

#endif
