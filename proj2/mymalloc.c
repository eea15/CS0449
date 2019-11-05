// Emma Akbari (eea21)
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include "mymalloc.h"

// USE THIS GODDAMN MACRO OKAY
#define PTR_ADD_BYTES(ptr, byte_offs) ((void*)(((char*)(ptr)) + (byte_offs)))

// Don't change or remove these constants.
#define MINIMUM_ALLOCATION  16
#define SIZE_MULTIPLE       8

unsigned int round_up_size(unsigned int data_size) {
	if(data_size == 0)
		return 0;
	else if(data_size < MINIMUM_ALLOCATION)
		return MINIMUM_ALLOCATION;
	else
		return (data_size + (SIZE_MULTIPLE - 1)) & ~(SIZE_MULTIPLE - 1);
}

// Node struct for each heap block header (doubly LL)
typedef struct Node {
	int value; // size of the data part, not including header
	bool used; // true if used
	struct Node* next;
	struct Node* prev;
} Node; // size = 16

// global variables to track heap
Node* head;
Node* tail;

// ------------------------------------------------------------------------------------------------------------------
// misc helper functions
// ------------------------------------------------------------------------------------------------------------------

// print values of all items in list
void list_print(){

	// print if list is empty, otherwise print list
	if(head == NULL) {
		printf("list empty\n");
	} else {
		// iterate through LL and print node values and their state
		for(Node* n = head; n != NULL; n = n->next){
			if(n->used) {
				printf("[U %d]", n->value);
				// only print arrow if not last node
				if(n->next != NULL) {
					printf(" -> ");
				}
			} else {
				printf("[F %d]", n->value);
				// only print arrow if not last node
				if(n->next != NULL) {
					printf(" -> ");
				}
			}
		}
		printf("\n");
	}
}

// iterate through headers to get size of LL
int list_size() {
	int numBlocks = 0;

	if(head != NULL) {
		for(Node* n = head; n != NULL; n = n->next) {
			numBlocks++;
		}
	}

	return numBlocks;
}

// ------------------------------------------------------------------------------------------------------------------
// helper functions for my_malloc
// ------------------------------------------------------------------------------------------------------------------

// set struct fields of a new header 
void fill_new_header(Node* n, int data_size) {
	n->value = data_size;
	n->used = true;
	n->next = NULL;
	n->prev = NULL;
}

// set struct fields of a reallocated header
void fill_header(Node* n) {
	n->used = true;
}

// set struct fields of free block during split
void fill_free_header(Node* n, int data_size, Node* next, Node* prev) {
	n->value = data_size;
	n->used = false;
	n->next = next;
	n->prev = prev;
}

// assign allocated Node as head and tail if empty LL
// return true if only node
bool assign_headtail(Node* n) {
	if(head == NULL && tail == NULL) {
		head = n;
		tail = n;
		return true;
	}
	return false;
}

// given a pointer to the header portion of a block, move to data
void* get_header_data(void* header_ptr) {
	return PTR_ADD_BYTES(header_ptr, sizeof(Node));
}

// if appending to end of list, assign node as new tail
void add_tail(Node* new_tail) {
	new_tail->prev = tail;
	new_tail->prev->next = new_tail;
	tail = new_tail;
	tail->next = NULL;
}

// iterate through the LL and find the biggest free block
Node* find_largest_free() {
	Node* bigBlock = NULL; // return NULL if all blocks are used or the list is empty
	int bigValue = 0;

	// find biggest block by comparing values
	for(Node* n = head; n != NULL; n = n->next) {
		if(n->value > bigValue && !n->used) {
			bigBlock = n;
			bigValue = n->value;
		}
	}

	return bigBlock;
}

// insert a new header into the LL 
void make_free_block(Node* old_header, int offset, int free_size) {
	// count size into block & put header there
	Node* freeBlock = PTR_ADD_BYTES(old_header, offset + sizeof(Node));
	fill_free_header(freeBlock, free_size, old_header->next, old_header);
	// insert into LL
	freeBlock->next->prev = freeBlock;
	old_header->next = freeBlock;
}

// ------------------------------------------------------------------------------------------------------------------
// helper functions for my_free
// ------------------------------------------------------------------------------------------------------------------

// given a pointer to the data portion of a block, move to header
void* get_data_header(void* data_ptr) {
	return PTR_ADD_BYTES(data_ptr, sizeof(Node)*-1);
}

// contract heap when the user is freeing the tail
void contract_heap() {
	Node* new_tail = tail->prev;
	brk(new_tail->next);
	tail = new_tail;
	tail->next = NULL;
}

// contract heap for the last remaining node/block
void contract_last_block(Node* lastBlock) {
	brk(lastBlock);
	head = NULL; 
	tail = NULL;
}

// coalesce a block with the one after it
void coalesce_next(Node* first, Node* middle, Node* last) {
	// calculate new size
	int new_size = first->value + middle->value + sizeof(Node);
	// remove middle header from LL
	first->next = last;
	// check if coalescing tail
	if(last == NULL) {
		tail = first; // reassign tail
	} else {
		last->prev = first;
	}
	// update size of new coalesced block
	first->value = new_size;
}

// coalesce a block with the previous one in the normal case, return pointer to it
Node* coalesce_prev(Node* first, Node* middle, Node* last) {
	// calculate new size
	int new_size = first->value + middle->value + sizeof(Node);
	// remove middle header from LL
	first->next = last;
	last->prev = first;
	// update size of coalesced block
	first->value = new_size;

	return first;
}

// coalesce tail with previous block and return pointer to it
Node* coalesce_tail(Node* new_tail, Node* old_tail) {
	// calculate new size
	int new_size = new_tail->value + old_tail->value + sizeof(Node);
	// remove tail from list
	tail = new_tail;
	tail->next = NULL;
	// update size of new coalesced block
	tail->value = new_size;

	return tail;
}

// ------------------------------------------------------------------------------------------------------------------
// my_malloc and my_free
// ------------------------------------------------------------------------------------------------------------------
void* my_malloc(unsigned int size) {
	if(size == 0)
		return NULL;
	
	size = round_up_size(size);

	// ------- Don't remove anything above this line. -------
	// Here's where you'll put your code!

	Node* header; // new block header to be allocated
	Node* biggestBlock = find_largest_free(); // find biggest free block

	// reallocate a free space if available using worst fit
	if(biggestBlock != NULL && biggestBlock->value >= size) {
		header = biggestBlock;

		// calculate what would be the size of a new free block
		int new_free_size = header->value - size - sizeof(Node);
		// only split block if the new free block wouldn't be too small
		if(new_free_size >= MINIMUM_ALLOCATION) {
			make_free_block(header, size, new_free_size);
			// update allocated block header
			header->value = size;
			header->used = true;
		} else {
			fill_header(header); // fill struct fields, use whole block
		}
	}
	else {
		// expand the heap if no usable blocks
		header = sbrk(size + sizeof(Node)); // newly allocated block accounts for header size
		fill_new_header(header, size); // fill struct fields
		bool only_node = assign_headtail(header); // assign block as head and tail if list empty
		// if not only node, set prev & reassign tail
		if(!only_node) {
			add_tail(header);
		}
	}

	// move pointer from header to data portion of block
	void* data = get_header_data(header);
	return data;
}

void my_free(void* ptr) {
	if(ptr == NULL)
		return;

	// and here's where you free stuff.

	// back up pointer to header
	Node* ptr_header = get_data_header(ptr);
	ptr_header->used = false; // mark header as free

	// coalescing
	// only coalesce if list size > 1
	if(list_size() > 1) {
		// special case: first block
		if(ptr_header == head) {
			// check if next block is free
			Node* next_block = ptr_header->next;
			Node* block_after = next_block->next;
			// if it is free, coalesce
			if(!next_block->used) {
				coalesce_next(ptr_header, next_block, block_after);
			}
		}
		else if(ptr_header == tail) {
			// special case: last block
			// check if previous block is free
			Node* prev_block = ptr_header->prev;
			// if it is free, coalesce
			if(!prev_block->used) {
				ptr_header = coalesce_tail(prev_block, ptr_header);
			}
		} else {
			// normal case
			// check if block before is free & if so coalesce
			Node* prev_block = ptr_header->prev;
			Node* next_block = ptr_header->next;
			if(!prev_block->used) {
				ptr_header = coalesce_prev(prev_block, ptr_header, next_block);
			}
			// check if block after is free & if so coalesce
			next_block = ptr_header->next;
			Node* block_after = next_block->next;
			// if it is free, coalesce
			if(!next_block->used) {
				coalesce_next(ptr_header, next_block, block_after);
			}
		}
	}

	// contract the heap if freeing the last block
	if(ptr_header == tail) {
		// if not freeing the only node, contract heap
		if(list_size() > 1) {
			contract_heap();
		}
		else { // only node, so reassign head and tail
			contract_last_block(ptr_header);
		}
	}
}
