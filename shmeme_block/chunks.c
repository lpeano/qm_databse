/*
 *
 *	Chunk Management procedure
 *
 *
 *
 */
#ifndef __CHUNK_MAMANGEMENT_
#define __CHUNK_MAMANGEMENT_
/*
 * This funcion get a record buffer and
 *
 * 	allocate necessary buffer chunks for it and then put values in it.
 *
 *      ptr is data buffer to be allocated.
 *	
 */
int init_chunks( void * ptr,unsigned long chunk_id , unsigned long chunk_size,unsigned long chunk_type) {
	CHUNK *chunk_ptr=NULL;
	unsigned long *chunks_ptr;
	if((chunks_ptr=malloc(sizeof())))
	get_chunk_ptr_list(chunk_size);

}
#endif
