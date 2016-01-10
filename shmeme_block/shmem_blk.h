#ifndef SHM__BLOCK__
#define SHM__BLOCK__
#include <sys/types.h>
#include <BitMap.h>

 extern int etext;
 extern int edata;
 extern int end;
/*
 * Block definiditions
 *
 *	Real block's buffer size is given from formula blk_size - sizeif(block_header)
 *
 */

#define N_BITMAP		8
#define BLOCK_BITMAP_SIZE (sizeof(U_1024_BYTE)*N_BITMAP)
#define BLOCK_BITMAP_BITS_SIZE (sizeof(U_1024_BYTE)*N_BITMAP*8)
typedef struct block_header {
	unsigned long long	blk_id;   	                        /* Unique blocl identifier */
	size_t			blk_size; 	                        /* Size of block in Bytes */
	size_t                  free_space;       	                /* Amount of free space */
	U_1024_BYTE		Chunk_BitMap[N_BITMAP];                 /* 0..7 chuncks 1KB BitMap (8192 bit) (0 - Free ; 1 - Used 
						                         * Total chunks = 64k
						                         */
	U_1024_BYTE 		Chunk_Record_BitMap[N_BITMAP];       	/*
									 * Bitmap of allocated Record start
						   			 */

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
#define GET_BLOCK( N, X, blk_size)   	((unsigned char* ) (v_shm_ptrs[X])+ (N*blk_size))


extern int init_blk( t_block * ,size_t ,unsigned long long );

extern size_t init_shmem_block( int, int , size_t ); 

/* Get free chunk bytes list in buffer */
/* Parameters  t_block *blk,int buffer_size , unsigned long *ptr */
extern long reclaim_free_byte_in_bitmap( t_block *,int  , long *);
extern long reclaim_free_block_space ( unsigned long area,unsigned long block_count, unsigned  long space, unsigned long blocksize);

extern key_t shm_key[];	
extern int   shm_id;	/* SharedMemory Identifier */ 

#endif
