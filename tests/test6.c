/********************************************************************************************************* 
 *
 * Test Number 6 - Heap reduction
 *
 * Description:
 * - Allocate 1024 bytes
 * - Allocation 512 bytes
 * - Allocate 1024 bytes
 * - Allocate 512 bytes
 * - Deallocate 1024 bytes (3rd allocation)
 * - Deallocate 512 bytes (4th allocation)
 *
 * Results:
 * - Sanity Check -> Heap usage at the end of program should be 1024 + 512 + 2*40 (Trackers) = 1616 bytes
 * - Expected     -> Max heap usage should be 1024 + 512 + 1024 + 512 = 3072 bytes
 * - Expected     -> Largest allocation should be 1024 bytes
 *
 ********************************************************************************************************/ 

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	void *ptr1, *ptr2, *ptr3, *ptr4;
	int  i;

	/* Perform allocations one by one */
	ptr1 = malloc(1024);
	ptr2 = malloc(512);
	ptr3 = malloc(1024);
	ptr4 = malloc(512);

	/* Free up 3rd and 4th allocation */
	free(ptr3);
	free(ptr4);

	return 0;
}
