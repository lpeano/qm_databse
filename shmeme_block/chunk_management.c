#include <stdlib.h>
#include <chunk_management.h>
#include <flog.h>

CH *CH_PTR;
/*
 * init_ch
 *
 * Initialize first block of chunk (NULL,NULL)
 *
 */
int int_ch( unsigned long ch_number,unsigned long blk_id) {
	if((CH_PTR=malloc(sizeof(CH)))!=(CH*) NULL) {
		CH_PTR->ch_number=ch_number;
		CH_PTR->prev=(CH*) NULL;
		CH_PTR->next=(CH*) NULL;
		CH_PTR->blk_id=blk_id;

	} else {
		return(-1);
	}
	return(0);
}

/*
 * add_ch
 *
 * Add next element to chunk list
 *
 */

int add_ch( unsigned long ch_number,unsigned long blk_id, unsigned long start, unsigned long size ) {
	CH *tmp_ch;
	tmp_ch=CH_PTR;
	/* Get Last element in the list */
	while(tmp_ch->next!=(CH*)NULL) {
		tmp_ch=tmp_ch->next;
	}	
	/* Allocate an other element */
	if((tmp_ch->next = malloc (sizeof(CH)) )!=(CH*) NULL) {
		((CH*)tmp_ch->next)->prev=tmp_ch;
		((CH*)tmp_ch->next)->next=NULL;
		((CH*)tmp_ch->next)->ch_number=ch_number;
		((CH*)tmp_ch->next)->blk_id=blk_id;
		((CH*)tmp_ch->next)->start=start;
		((CH*)tmp_ch->next)->size=size;
	} else {
		return(-1);
	}
	return(0);	
}

/*
 * delet_ch
 *
 * Delete all list
 *
 */

int delete_ch() {
	CH * tmp_ch;
	tmp_ch=CH_PTR;
	while(tmp_ch->next!=(CH*)NULL) {
		tmp_ch=tmp_ch->next;
	}	
	while( CH_PTR!=tmp_ch) {
		tmp_ch=tmp_ch->prev;
		free((CH*) tmp_ch->next);
	}
	free((CH*) CH_PTR);
	return(0);
}

/*
 * delet_ch
 *
 * Get last element of list
 *
 */
CH* get_last(){
	CH* tmp_ch;
	tmp_ch=(CH*) CH_PTR;
	while(tmp_ch->next!=(CH*) NULL) {
		tmp_ch=tmp_ch->next;
	}
	return(tmp_ch);
}

void debug_dump_ch() {
	CH *tmp;
	tmp=CH_PTR;
	if(log_level>=LOG_DEBUG4) {
		flog(LOG_DEBUG4,"DUMPING CH STRUCTURE LIST");
		do{
			flog(LOG_DEBUG4,"Pointer to element :%20p ch_number : %lu\n\t\tptr->prev:%20p ptr->next:%20p\n\t\tptr->start:%lu ptr->size:%lu",tmp,tmp->ch_number,tmp->prev,tmp->next,tmp->start,tmp->size);
		}while(tmp->next!=(CH *) NULL);
	}
}

