#include <shmem_blk.h>
#ifndef SHM_LINKED_LIST
#define SHM_LINKED_LIST
struct linked_list {
	t_block 		block;	 /* Data Block     	       */
	t_block 		*t_prev; /* Previe Block 	       */
					 /* If Null this is the firest */
	t_block  		*t_next; /* Next datablock */
					 /* If NULL this is the last   */
};
#endif
