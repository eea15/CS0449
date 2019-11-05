// Emma Akbari (eea21)
#include <stdio.h>
#include <unistd.h>

#include "mymalloc.h"

int main() {
	// You can use sbrk(0) to get the current position of the break.
	// This is nice for testing cause you can see if the heap is the same size
	// before and after your tests, like here.

	// testing allocating and deallocating in reverse order
	printf("allocate and deallocate in reverse order:\n");

	void* heap_at_start = sbrk(0);
	printf("At start: break at address %p\n", sbrk(0));
	printf("\n");

	void* block = my_malloc(100);
	printf("After allocation: break at address %p\n", sbrk(0));
	list_print();
	printf("\n");

	void* block2 = my_malloc(60);
	printf("After allocation: break at address %p\n", sbrk(0));
	list_print();
	printf("\n");

	void* block3 = my_malloc(80);
	printf("After allocation: break at address %p\n", sbrk(0));
	list_print();
	printf("\n");

	my_free(block3);
	printf("After free: break at address %p\n", sbrk(0));
	list_print();
	printf("\n");

	my_free(block2);
	printf("After free: break at address %p\n", sbrk(0));
	list_print();
	printf("\n");

	my_free(block);
	printf("After free: break at address %p\n", sbrk(0));
	list_print();
	printf("\n");

	void* heap_at_end = sbrk(0);
	unsigned int heap_size_diff = (unsigned int)(heap_at_end - heap_at_start);

	if(heap_size_diff)
		printf("Hmm, the heap got bigger by %u (0x%X) bytes...\n", heap_size_diff, heap_size_diff);

	// ADD MORE TESTS HERE.

	// testing coalescing by freeing last block
	printf("testing coalescing by freeing last block\n");
	
	heap_at_start = sbrk(0);
	printf("At start: break at address %p\n", sbrk(0));
	printf("\n");

	block = my_malloc(100);
	printf("After allocation: break at address %p\n", sbrk(0));
	list_print();
	printf("\n");

	block2 = my_malloc(60);
	printf("After allocation: break at address %p\n", sbrk(0));
	list_print();
	printf("\n");

	my_free(block);
	printf("After free: break at address %p\n", sbrk(0));
	list_print();
	printf("\n");

	my_free(block2);
	printf("After free: break at address %p\n", sbrk(0));
	list_print();
	printf("\n");

	heap_at_end = sbrk(0);
	heap_size_diff = (unsigned int)(heap_at_end - heap_at_start);

	if(heap_size_diff)
		printf("Hmm, the heap got bigger by %u (0x%X) bytes...\n", heap_size_diff, heap_size_diff);

	// testing coalescing by freeing misc blocks
	printf("testing coalescing by freeing misc blocks\n");
	
	heap_at_start = sbrk(0);
	printf("At start: break at address %p\n", sbrk(0));
	printf("\n");

	block = my_malloc(100);
	printf("After allocation: break at address %p\n", sbrk(0));
	list_print();
	printf("\n");

	block2 = my_malloc(60);
	printf("After allocation: break at address %p\n", sbrk(0));
	list_print();
	printf("\n");

	block3 = my_malloc(80);
	printf("After allocation: break at address %p\n", sbrk(0));
	list_print();
	printf("\n");

	void* block4 = my_malloc(20);
	printf("After allocation: break at address %p\n", sbrk(0));
	list_print();
	printf("\n");

	my_free(block2);
	printf("After free: break at address %p\n", sbrk(0));
	list_print();
	printf("\n");

	my_free(block3);
	printf("After free: break at address %p\n", sbrk(0));
	list_print();
	printf("\n");

	my_free(block4);
	printf("After free: break at address %p\n", sbrk(0));
	list_print();
	printf("\n");

	my_free(block);
	printf("After free: break at address %p\n", sbrk(0));
	list_print();
	printf("\n");

	heap_at_end = sbrk(0);
	heap_size_diff = (unsigned int)(heap_at_end - heap_at_start);

	if(heap_size_diff)
		printf("Hmm, the heap got bigger by %u (0x%X) bytes...\n", heap_size_diff, heap_size_diff);
	
	// testing splitting
	printf("testing splitting\n");
	
	heap_at_start = sbrk(0);
	printf("At start: break at address %p\n", sbrk(0));
	printf("\n");

	block = my_malloc(100);
	printf("After allocation: break at address %p\n", sbrk(0));
	list_print();
	printf("\n");

	block2 = my_malloc(60);
	printf("After allocation: break at address %p\n", sbrk(0));
	list_print();
	printf("\n");

	block3 = my_malloc(80);
	printf("After allocation: break at address %p\n", sbrk(0));
	list_print();
	printf("\n");

	my_free(block2);
	printf("After free: break at address %p\n", sbrk(0));
	list_print();
	printf("\n");

	block4 = my_malloc(10);
	printf("After allocation: break at address %p\n", sbrk(0));
	list_print();
	printf("\n");

	my_free(block);
	printf("After free: break at address %p\n", sbrk(0));
	list_print();
	printf("\n");

	void* block5 = my_malloc(40);
	printf("After allocation: break at address %p\n", sbrk(0));
	list_print();
	printf("\n");

	my_free(block4); // 10
	printf("After free: break at address %p\n", sbrk(0));
	list_print();
	printf("\n");

	my_free(block5); // 40
	printf("After free: break at address %p\n", sbrk(0));
	list_print();
	printf("\n");

	my_free(block3); // 80
	printf("After free: break at address %p\n", sbrk(0));
	list_print();
	printf("\n");

	heap_at_end = sbrk(0);
	heap_size_diff = (unsigned int)(heap_at_end - heap_at_start);

	if(heap_size_diff)
		printf("Hmm, the heap got bigger by %u (0x%X) bytes...\n", heap_size_diff, heap_size_diff);
	
	return 0;
}
