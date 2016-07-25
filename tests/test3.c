/**********************************************************************************
 *
 * Test Number 3 : Out of Memory (Restriction of the current version)
 *
 * Description:
 * - Allocate 950KB
 * - Allocate 950KB
 * - Allocate 512KB
 * 
 * Results:
 * - Santiy Check -> Program will exit
 * - Expected     -> First two allocation should succeed
 * - Expected     -> Thrid allocation should fail with message : "Out of Memory!"
 *
 *********************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#define NUM_OF_ALLOCATIONS 3
#define SIZE_OF_ALLOCATION 950 * 1024

int main(void)
{
	void **ptr_array;
	int  i;

	/* Allocate an array of 1024 pointers */
	ptr_array = malloc(NUM_OF_ALLOCATIONS * sizeof(void*));

	/* Against each pointer in pointer array, allocate 1024 bytes */
	for (i = 0; i < (NUM_OF_ALLOCATIONS - 1); i++) {
		ptr_array[i] = malloc(SIZE_OF_ALLOCATION);
	}

	/* Perform the last allocation */
	ptr_array[2] = malloc(512 * 1024);

	/* Now deallocate each pointer in the pointer array */
	for (i = 0; i < NUM_OF_ALLOCATIONS ; i++) {
		free(ptr_array[i]);
	}

	/* Finally free the array of pointers */
	free(ptr_array);

	return 0;
}
