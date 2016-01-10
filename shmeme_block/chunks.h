#ifndef __CHUNK_RECORDS_
#define __CHUNK_RECORDS_
#include <sys/time.h>
typedef struct chunk {
	unsigned long chunk_id;	  /* Define unique identifier to this record
				   * 
				   */

	unsigned long chunk_size; /* Size of data_buffer*/

	unsigned long chunk_type; /* Type defining record type 
				   * #include <rtype.h> 
				   *
				   *
				   */  

	struct timeval timpestamp; /*
				    * Timestamp of data defined when data is changed.
				    *
				    *
				    */
	/* Virtual memory Mapping */
	unsigned long prev;        /*
				    * Preview element of record chain. 
				    *
				    *	NULL	: First buffer.
				    *	not NULL: If internal part of chain buffer.
				    *
				    * Value rappresent offset bytes from block data buffer initial pointer	
				    *
				    */

	unsigned long next;	   /*
				    * Next element of record chain.
				    *
				    * 	NULL	: If last element of chain buffer.
				    * 	not NULL: If there is an other data in buffer chain.
				    *
				    * Value rappresent offset bytes from block data buffer initial pointer	
				    *
				    */
	unsigned char  data_buffer[];
} t_chunk;

/*
 *
 *	Macro chunks's management
 *
 */
#define GET_NEXT_CHUNK_ADDR(ptr) ({__typeof__(prt) __ptr=ptr;\
					___ptr->next})
#define GET_PREV_CHUNK_ADDR(ptr) ({__typeof__(prt) __ptr=ptr;\
					___ptr->prev})

#endif
