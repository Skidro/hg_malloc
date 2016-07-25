/**************************************************************************************************** 
 * 
 * Test Number 4 : Reusage of Tracker
 *
 * Description:
 * - Allocate 1024 bytes
 * - Allocate 512 bytes
 * - Allocate 1024 bytes
 * - Deallocate 512 bytes
 * - Allocate 512 bytes
 * - Deallocate all memory
 *
 * Results:
 * - Sanity Check -> Heap usage at the end of program should be zero
 * - Expected     -> The address returned for 4th allocation (i.e. 512 bytes) should be the same as
 *                   the address returned for 2nd allocation
 * - Expected     -> Max heap usage should be 1024 + 1024 + 512 = 2560 bytes
 * - Expected     -> Largest allocation should be 1024 bytes
 * 
 ****************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main(void)
{
	void *ptr1, *ptr2_0, *ptr2_1, *ptr3;
	int  i;

	/* Perform allocations one by one */
	ptr1 = malloc(1024);
	ptr2_0 = malloc(512);
	ptr3 = malloc(1024);

	/* Deallocate second allocation */
	free(ptr2_0);

	/* Allocate 512 bytes once again */
	ptr2_1 = malloc(512);

	/* Make sure that the program returned correct value */
	assert(ptr2_0 == ptr2_1);

	/* Now deallocate each pointer in the pointer array */
	free(ptr1);
	free(ptr2_1);
	free(ptr3);

	return 0;
}
