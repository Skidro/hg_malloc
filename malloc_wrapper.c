/**********************************************************************************************************************
 * Dynamic Memory Allocation Using Huge Pages
 *
 * This file provides a wrapper for libc malloc. The custom implementation of malloc present in this file uses
 * huge pages to provide the requesting application with non-fragmented physically contiguous memory range.
 * The purpose for using this kind of allocation scheme is to reduce the variation in miss-rate that an
 * application sees from run-to-run due to disparity in the virutal memory pages allocated by the OS
 *********************************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include "list.h"

/*********************************************
 * Macro Definitions
 *********************************************/

/* This macro defines the size of one huge page */
#define SYS_HUGE_PAGE_SIZE	2048 * 1024

/* This macro calculates the address of a memory chunk from a given tracker address */
#define MEM_GET_ADDRESS(track_ptr)									\
		(void *)((unsigned long)track_ptr + (unsigned long)(sizeof(track_t)))

/* This is the converse of the above macro. It gets the tracker from the address of a memory chunk */
#define MEM_GET_TRACKER(addr_ptr)									\
		(void *)((unsigned long)addr_ptr - (unsigned long)(sizeof(track_t)))

/* This macro calculates the total size of the memory region we have for allocation */
#define MEM_GET_SIZE(mem_start)										\
		((unsigned long)(mem_start) + (unsigned long)(SYS_HUGE_PAGE_SIZE))

/* This macro calculates the current end of the memory region 
   CAUTION : The list must not be empty */
#define MEM_GET_END()											\
		(void *)((unsigned long)((container_of(alloc_list.prev, track_t, list))->address) 	\
			+ (unsigned long)((container_of(alloc_list.prev, track_t, list))->size))
		
/*********************************************
 * Global Data
 ********************************************/

typedef struct {
	struct list_head 	list;
	void   			*address;
	unsigned long 		size;
	unsigned long 		free;
} track_t;

static struct list_head alloc_list;
static void    		*g_mem_ptr;
static int 		init = 0;

/*********************************************
 * Helper Functions
 ********************************************/

/*
 *
 * Name:
 * populate_tracker
 * 
 * Description:
 * This is a helper function for populating a tracker i.e. malloc-header
 * with the information regaring the on-going memory allocation
 *
 */
static inline void populate_tracker(track_t *tracker, unsigned long size)
{
	/* Add this allocated chunk to tracker list */
	INIT_LIST_HEAD(&tracker->list);
	list_add_tail(&tracker->list, &alloc_list);

	/* Populate the tracker with information about this allocation */
	tracker->size = size;
	tracker->address = MEM_GET_ADDRESS(tracker);
	tracker->free = 0;
	return;
}


/*********************************************
 * Function Definitions
 ********************************************/

/* 
 *
 * Name:
 * __wrap_malloc
 *
 * Description:
 * This function intercepts the call to malloc and performs
 * dynamic memory allocation on behalf of the caller
 *
 */
void *__wrap_malloc(size_t size)
{
	track_t		*tracker = NULL;
	void		*mem_end = NULL;

	/* Find out if this is the first call to malloc */
	if (init == 0) {
		init = 1;
		
		/* Initialize a linked list to track allocations */
		INIT_LIST_HEAD(&alloc_list);

		/* Allocate one huge page to take care of all the memory requests of this application */
		g_mem_ptr = mmap(0, SYS_HUGE_PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);

		/* Verify that the allocation was successful */
		if (g_mem_ptr == MAP_FAILED) {
			perror("alloc failed");
			exit(1);
		}

		/* Output some debug information regarding the start of memory region */
		// printf("Memory Region     : %p\n", g_mem_ptr);

		/* Place a malloc header at the start of the memory area */
		tracker = (track_t *)g_mem_ptr;

		/* Populate the tracker with information regaring this allocation */
		populate_tracker(tracker, (unsigned long)size);

		/* Nothing needs to be done for initializtion case */
		goto done;
	}

	/* Look through the allocated chunks to find an appropriate sized one which is free */
	list_for_each_entry(tracker, &alloc_list, list) {
		if (tracker->free && tracker->size >= size) {
			/* Found the right chunk */
			tracker->free = 0;

			/* Return the tracker to caller */
			goto done;
		}
	}

	/* No free chunk of the right size available. Allocate a chunk in the memory page */
	if (list_empty(&alloc_list)) {
		/* List is empty so create the first chunk */
		tracker = (track_t *)g_mem_ptr;

		/* Populate the tracker with information regaring this allocation */
		populate_tracker(tracker, (unsigned long)size);

		/* Return the address to the caller */
		goto done;
	} else {
		/* List is not empty. Look for the last element in the list */
		mem_end = MEM_GET_END();

		/* Make sure that we have enough memory */
		if ((unsigned long)MEM_GET_ADDRESS(mem_end) + (unsigned long)size >= MEM_GET_SIZE(g_mem_ptr)) {
			/* Out of Memory!!! */
			printf("We are out of Memory!\n");

			/* Exit the program */
			exit(-1);
		}

		/* Place a tracker at the current end of memory area */
		tracker = (track_t *)mem_end;

		/* Populate the tracker with information regaring this allocation */
		populate_tracker(tracker, (unsigned long)size);

		/* Return the address to the caller */
		goto done;
	}

done:

	/* Display some debug information */
	// printf("Returning Address : %p\n", tracker->address);

	/* Return the address to caller */
	return tracker->address;
}

/* 
 *
 * Name:
 * __wrap_free
 *
 * Description:
 * This function intercepts the call to free. It frees the allocated memory
 * and performs defragmentation whenever possible 
 *
 */
void __wrap_free(void *ptr)
{
	track_t *tracker;

	/* Get the tracker from the address */
	tracker = MEM_GET_TRACKER(ptr);

	/* Mark the tracker as free */
	tracker->free = 1;

	// printf("Tracker Address   : %p\n", tracker);
	// printf("Freeing Address   : %p | %p\n", ptr, tracker->address);

	/* If the tracker is the last chunk in the heap, then delete it */
	if ((tracker->list).next == &alloc_list) {
		list_del_init(&tracker->list);
	}

	printf("Heap Usage        : %lu Bytes\n", (list_empty(&alloc_list))? 0 : (unsigned long)MEM_GET_END() - (unsigned long)g_mem_ptr);

	return;
}
