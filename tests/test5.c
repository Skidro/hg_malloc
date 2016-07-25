/**********************************************************************************************************
 * 
 * Test Number 5 : Fragmentation
 *
 * Description:
 * - Allocate 1024 bytes
 * - Allocate 512 bytes
 * - Deallocate 1024 bytes
 *
 * Results:
 * - Sanity Check -> Heap size at the end of program should be 1024 + 512 + 2*40 (Trackers) = 1616 bytes
 * - Expected     -> Max heap usage should be 1024 + 512 = 1536 bytes
 * - Exp cted     -> Largest allocation should be 1024 bytes
 * 
 *********************************************************************************************************/ 

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	void *ptr1, *ptr2;
	int  i;

	/* Perform allocations one by one */
	ptr1 = malloc(1024);
	ptr2 = malloc(512);

	/* Deallocate second allocation */
	free(ptr1);

	return 0;
}
