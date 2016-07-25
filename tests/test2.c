/****************************************************************************************
 *
 * Test Number 2 : Multiple Allocations
 *
 * Description:
 * - Allocate an array of 1024 pointers - 8KB
 * - For each allocated pointer, allocate 1024 bytes - 1024 x 1024 = 1MB
 * - Free up all the pointers one-by-one
 * - Free up the pointer array
 * 
 * Results:
 * - Sanity Check -> Heap usage at the end of program should be zero
 * - Expected     -> Max memory usage should be 8KB + 1MB = 1024 x 1024 + 8 x 1024 = 1056768 bytes
 * - Expected     -> Largest allocation should be 8 Kbytes
 *
 ****************************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#define NUM_OF_ALLOCATIONS 1024
#define SIZE_OF_ALLOCATION 1024

int main(void)
{
	void **ptr_array;
	int  i;

	/* Allocate an array of 1024 pointers */
	ptr_array = malloc(NUM_OF_ALLOCATIONS * sizeof(void*));

	/* Against each pointer in pointer array, allocate 1024 bytes */
	for (i = 0; i < NUM_OF_ALLOCATIONS; i++) {
		ptr_array[i] = malloc(SIZE_OF_ALLOCATION);
	}

	/* Now deallocate each pointer in the pointer array */
	for (i = 0; i < NUM_OF_ALLOCATIONS ; i++) {
		free(ptr_array[i]);
	}

	/* Finally free the array of pointers */
	free(ptr_array);

	return 0;
}
