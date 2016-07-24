#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	/* Allocate and free dynamic memory */
	free(malloc(1024));

	return 0;
}
