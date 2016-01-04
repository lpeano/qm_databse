#ifndef SHM__BLOCK__
#define SHM__BLOCK__
#include <sys/types.h>

/*
 * Block definiditions
 *
 *	Real block's buffer size is given from formula blk_size - sizeif(block_header)
 *
 */

typedef struct block_header {
	unsigned long 		blk_id;   /* Unique blocl identifier */
	size_t			blk_size; /* Size of block in Bytes */
} t_block_header;

typedef struct block {
	t_block_header		block_header;
	void *			buffer[]; 	/* Data Buffer */
} t_block;

/*
 *
 * Block Management Macros
 *
 */
/*
 * Get pointer to Nth block of area X
 */
#define GET_BLOCK ( N ) ( X )   	v_shm_ptrs[X]+(N*blk_size)


extern int init_blk( t_block * ,size_t ,int );

extern size_t init_shmem_block( int, int , size_t ); 


extern key_t shm_key[];	
extern int   shm_id;	/* SharedMemory Identifier */ 

#endif
