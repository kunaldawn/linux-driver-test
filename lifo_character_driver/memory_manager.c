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

#include <linux/slab.h>
#include "memory_manager.h"

int allocate_new_block(memblock * head) {
	// Temporary memblock pointer
	memblock *temp, *block;
	// get the last block if any
	temp = get_last_block(head);
	// allocate memory
	block = kmalloc(sizeof(memblock), GFP_KERNEL);
	// check if allocation was success
	if (block == NULL) {
		// return error code
		return 1;
	}
	// allocate memory
	block->data = kmalloc(sizeof(char) * DRIVER_DEFAULT_BLOCK_SIZE,
	GFP_KERNEL);
	// check if allocation was success
	if (block->data == NULL) {
		// free previously allocated memory
		kfree(block);
		// return error code
		return 1;
	}
	// set newly allocated block to be last block
	block->next = NULL;
	// init block data
	memset(block->data, 0x00, sizeof(char) * DRIVER_DEFAULT_BLOCK_SIZE);
	// attach block to last block
	temp->next = block;
	return 0;
}

memblock * get_last_block(memblock *head) {
	// Temporary memblock pointer
	memblock *temp;
	// assign temporary variable
	temp = head;
	// iterate through linked list to get last block
	while (temp->next != NULL) {
		// move to next block
		temp = temp->next;
	}
	return temp;
}

void free_all_blocks(memblock *head) {
	// temporary memblock pointers
	memblock *temp, *temp1;

	// check if any blocks available
	if (head->next == NULL) {
		// return
		return;
	}
	// go to first block
	temp = head->next;
	// iterate through all blocks
	while (temp != NULL) {
		// store next blocks pointer
		temp1 = temp->next;
		// free blocks data
		kfree(temp->data);
		// free block
		kfree(temp);
		// set next block
		temp = temp1;
	}
}
