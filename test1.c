/*********************************************************************** 
 * 
 * Test Number 1 - Simple Test to Verify Allocation and Deallocation
 *
 * Description:
 * - Allocate 1024 bytes
 * - Deallocate 1024 bytes
 *
 * Results:
 * - Sanity Check -> Heap usage at the end of program should be zero
 * - Expected     -> Max memory usage should be 1024 bytes
 * - Expected     -> Largest allocation should be 1024 bytes
 * 
 **********************************************************************/

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	/* Allocate and free dynamic memory */
	free(malloc(1024));

	return 0;
}
