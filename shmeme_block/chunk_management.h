#ifndef __CHUNK_MAMANGEMENT__
#define __CHUNK_MAMANGEMENT__

/*
 * This structure is intended to manage bitmap allocation 
 * It rappresent bit chunk allocation in one or more block for particular chunks of records.
 * It is used from reclaime process memory de/allocator.
 *
 */
typedef struct ch {
	unsigned long ch_number;		/* Sequential number of list */
	struct ch * prev;			/* Prev element in list 
						 *
						 * 	NUL in case of first element
						 */
      	struct ch * next;			/* Next element of list
						 * 
						 * 	NULL in case of this is last element of list
						 */ 
	unsigned long blk_id;			/* Block Identifier of chunk in Memory */
	unsigned long start;			/* Starter byte of chunk */
	unsigned long size;			/* Size intended for start + N elements (Inclusive) */

} CH;
#endif

extern CH *CH_PTR;
extern int 	int_ch( unsigned long ,unsigned long );
extern int 	add_ch( unsigned long ,unsigned long , unsigned long , unsigned long  );
extern void 	debug_dump_ch(void);
extern int 	delete_ch(void);
extern CH * get_last();
