#include <shmem_linked_list.h>
#include <shmem_blk.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <stdio.h>
#include <flog.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
#include <chunks.h>
#include <BitMap.h>
#include <stdlib.h>
#include <shmem_blk.h>
#include <chunk_management.h>
/* 
 * This function initialize newely created block 
 *
 *
 */
#define _PERM 0777
#define PERM  _PERM

/*
 * Definse 1000 possible key of blocks buffers
 */
key_t shm_key[1000];
/*
 * Definse 1000 possible shm_id of blocks buffers
 */
int   v_shm_id[1000];    /* SharedMemory Identifier */
/*
 * Definse 1000 possible pointer of shm blocks buffers
 */
t_block *v_shm_ptrs[1000];

int init_blk( t_block *blk ,size_t block_size,unsigned long long block_id)
{
	int c=0;

	/* To be implemented */
	blk->block_header.blk_size=block_size;
	blk->block_header.free_space=block_size-sizeof(t_block_header);
	blk->block_header.blk_id=block_id;
	/* Clean all bytes */
	memset((char*)blk->buffer,0,block_size-sizeof(t_block_header));
	//memset((char *) blk->buffer,0,block_size);
	*((unsigned char *)&blk->block_header.Chunk_BitMap[0])&=(unsigned char) 0X00;
	*((unsigned char *)&blk->block_header.Chunk_BitMap[1])&=(unsigned char) 0X00;
	*((unsigned char *)&blk->block_header.Chunk_BitMap[2])&=(unsigned char) 0X00;
	*((unsigned char *)&blk->block_header.Chunk_BitMap[3])&=(unsigned char) 0X00;
	*((unsigned char *)&blk->block_header.Chunk_BitMap[4])&=(unsigned char) 0X00;
	*((unsigned char *)&blk->block_header.Chunk_BitMap[5])&=(unsigned char) 0X00;
	*((unsigned char *)&blk->block_header.Chunk_BitMap[6])&=(unsigned char) 0X00;
	*((unsigned char *)&blk->block_header.Chunk_BitMap[7])&=(unsigned char) 0X00;
	for(c=0;c<sizeof(blk->block_header.Chunk_BitMap);c++) {
		flog(LOG_DEBUG4,"Bitmap offset %lu",GET_INDEX_BITMAP((c)));
		unset_bit((U_1024_BYTE*)(&blk->block_header.Chunk_BitMap[GET_INDEX_BITMAP((c))]),(c)%1024);
	}
	//for( c=block_size ;c>blk->block_header.free_space;c--) {
		
	//}
	return(0);
}
/*
 * Definse 1000 possible size of shm blocks buffers
 */
size_t blk_area_size[1000];

/*
 * This funcion search for n bytes chunks in block's bitmap.
 * It is called from reclaim_free_block_space 
 *
 *
 */
long reclaim_free_byte_in_bitmap( t_block *blk,int buffer_size ) {
	/*
	 *
	 *	Searching chunk_size + buffer size-1 bytes
	 *
	 */
	int chun_minimum_size=sizeof(t_chunk);
	unsigned long bit;
	U_1024_BYTE *BitMap_ptr=NULL;

	BitMap_ptr=(U_1024_BYTE *) blk->block_header.Chunk_BitMap;
	unsigned long c=0,number_of_bytes=0;
	CH *last;
	for (c=0;(c<BLOCK_BITMAP_BITS_SIZE) && (number_of_bytes<buffer_size);c++) {
		/* Locking for minimum chunks bytes  */
		long n=0;
		flog(LOG_DEBUG4,"Starting searching reclaimed %lu bytes from bitmap pointer %lu of %lu",buffer_size,c,BLOCK_BITMAP_BITS_SIZE);
		flog(LOG_DEBUG4,"Minimum chunk size is %lu",(unsigned long)sizeof(t_chunk));
		for(n=0;n<chun_minimum_size;n++,c++) {
			bit=GET_BLK_BIT(BitMap_ptr,c);	
			flog(LOG_DEBUG4,"Getting bit number c:%lu n:%ld=> %lu for chunk",c,n,bit);
			if(bit==1) {
				n=n*(-1);
				break;
			}
		}
		if (n>0) {
			/* Getted minimum chunk size data
			 * now continuing after allocating
			 */ 
			flog(LOG_DEBUG4,"Matched minimum chunk size");
			/* Variable bitmap allcoator */
			last=(CH *) get_last();
			if( last==(CH *) NULL) {
				flog(LOG_ERROR,"Last ch pointer is NULL !!! Somthinkg goes bad!!!");
			}	
			last->start=c-n;	/* Set first pointer in buffer */
			last->size=n;		/* Set actual size in buffer   */
			number_of_bytes=1;
			c++;
			/*
			 * Now allocating remaining values
			 *
			 */
			long m=c;
			for(m=c;(number_of_bytes<(buffer_size)) && m<BLOCK_BITMAP_BITS_SIZE ;c++,m++) {
				bit=GET_BLK_BIT(BitMap_ptr,m);	
				flog(LOG_DEBUG4,"Getting %lu on BitMap value is %lu number of bytes %lu max number of bytes %lu",m,bit,number_of_bytes,buffer_size);
				if(bit==1) {
					n=-1;
					/* Add new chunk elment */
					add_ch( last->ch_number+1,last->blk_id, 0, 0 ); 
					flog(LOG_DEBUG4,"After bit number %lu no chunk founded restart finding creating new chunk",c);
				} else {
					flog(LOG_DEBUG4,"Getting new byte at offset %lu for buffer_size:%lu number_of_bytes:%lu",c,buffer_size,number_of_bytes);
					last->size++; 
					number_of_bytes++;
					if( number_of_bytes==(buffer_size) ) {
						return (number_of_bytes);	
					}
				}
			}	

		} else {
			flog(LOG_DEBUG4,"After bit number %lu no chunk founded",c);
		}
	}
	return((-1));
}

/*
 * reclaim_free_block_space(block_id,space)
 *
 *
 *
 */
long reclaim_free_block_space ( unsigned long area,unsigned long bcount, unsigned  long space, unsigned long block_size,void *buffer) {
	t_block *block=(t_block *)NULL;
	long ret_recl;
	t_chunk *chunk_ptr=(t_chunk *) NULL;


	block=(t_block *)GET_BLOCK(bcount,area,block_size);
	flog(LOG_DEBUG4,"Block number %lu  of size %10lu is pointed by %p",bcount,block_size,(char *) block);
	flog(LOG_DEBUG4,"Free space in block : %llu",block->block_header.free_space);
	if( (space+sizeof(t_chunk)) <= block->block_header.free_space ) {
		flog(LOG_DEBUG4,"Block space is appropriate to allocate %lu bytes",space);	
		/* Critical region start here */
		
		/* Initing first list element */
		if( int_ch(1,bcount)<0) {
			flog(LOG_ERROR,"Error allocating first ch in reclaim_free_block_space %s",strerror(errno));
			return(-1);
		} else {
			flog(LOG_DEBUG4,"Allocated CH_PTR first chunk to %20p",CH_PTR);
		}	
		if((ret_recl=reclaim_free_byte_in_bitmap(block,space))<0) {
			flog(LOG_ERROR,"Error in Funciont reclaim_free_byte_in_bitmap\n\tRecalining %lu bytes retriving return code %ld",space,ret_recl);
			delete_ch();	/* Garbage Collection */
			return (-2);
		} else {
			flog(LOG_DEBUG3,"Function reclaim_free_byte_in_bitmap reclaiming %lu returned %ld",space,ret_recl);
			debug_dump_ch();
			set_bit((U_1024_BYTE*)(&block->block_header.Chunk_BitMap[GET_INDEX_BITMAP(CH_PTR->start)]),CH_PTR->start%1024);
			set_bit((U_1024_BYTE*)(&block->block_header.Chunk_Record_BitMap[GET_INDEX_BITMAP(CH_PTR->start)]),CH_PTR->start%1024);
			do{
				int c;
				for(c=0;c<CH_PTR->size;c++) {
					flog(LOG_DEBUG4,"Bitmap offset %lu",GET_INDEX_BITMAP((CH_PTR->start+c)));
					set_bit((U_1024_BYTE*)(&block->block_header.Chunk_BitMap[GET_INDEX_BITMAP((CH_PTR->start+c))]),(CH_PTR->start+c)%1024);
					block->block_header.free_space--;
				}
				int chn=0;
				/* Copy data tu buffer */
				chunk_ptr=(t_chunk*)(block->buffer+CH_PTR->start+chn);
				chunk_ptr->chunk_id=CH_PTR->ch_number;
				chunk_ptr->chunk_size=CH_PTR->size;
				chunk_ptr->next=-1;
				for(chn=(CH_PTR->start+sizeof(t_chunk));chn<CH_PTR->size;chn++) {
					*((char *) chunk_ptr->data_buffer+chn)=*((char *) buffer+chn);
				}
				if(CH_PTR->next!=(CH*) NULL) { 
					chunk_ptr->next=(CH_PTR->next)->start;
				}
			} while (CH_PTR->next!=(CH*) NULL);
			delete_ch();	/* Garbage Collection */
		}	
		/* Critical region terminate here */
	} else {
		flog(LOG_DEBUG4,"Block space is inappropriete to allocate %lu bytes",space);
		return ((block->block_header.free_space-space));
	}

	return(0);
}
/*
long release_block_space(unsigned long area , unisgned long bcount, unsigned long chunk_id ) {
	t_block *block=(t_block *) NULL;
	t_chunk chunk;

	block=(t_block *)GET_BLOCK(bcount,area,block_size);		
	flog(LOG_DEBUG4,"Block number %lu  of size %10lu is pointed by %p",bcount,block_size,(char *) block);
	flog(LOG_DEBUG4,"Free space in block : %llu",block->block_header.free_space);
	if(get_chunk_addr(block,chunk_id)>0) {
		flog(LOG_DEBUG4,"Chunk id %lu founded in block",chunk_id);
		flog(LOG_DEBUG4,"Starting parsing chunk list");
	} else {
		flog(LOG_ERROR,"No chunk id %lu founded in block",chunk_id);
	}
	return(0);
}a*/

/*
 * Templete memory allocation.
 *
 *
 *
 *
 */
size_t init_shmem_block( int area, int block_number, size_t block_size ) {
	int c,errn=0,init_b_flag=0;
	size_t total_memeory_size=0;
	char *ptr=(char *)0x40000000;
	char *last=(char *)0xC0000000;
	char *last1;
	t_block *shmptrtmp;
	flog(LOG_INFO,"Memory Mapping for:\n\t etext :%10p\n\t edata :%10p\n\t eend :%10p",&etext,&edata,&end);
	if ( brk(last)<0) {
		flog(LOG_INFO,"Error moving heap break to %20p \"%s\" ",last,strerror(errno));
	}
	last1=0;
	flog(LOG_INFO,"Last Breack memory point is %10p %llu %10p %llu",last,(unsigned long long) last,last1,(unsigned long long) last1);
	flog(LOG_INFO,"Memory Mapping for:\n\t etext :%10p\n\t edata :%10p\n\t eend :%10p",&etext,&edata,&end);
	blk_area_size[area]=block_size;
	total_memeory_size=block_number*block_size+(sizeof(t_block)*block_number);
	shm_key[area]=ftok("/tmp",area);
	flog(LOG_DEBUG2,"Shared Memeory Key is : %LX",shm_key[area]);
	if((v_shm_id[area]=shmget(shm_key[area],total_memeory_size,SHM_NORESERVE|IPC_CREAT|PERM))<0) {
		flog(LOG_DEBUG2,"Returning from shmget errno is %d error %s ",errno,strerror(errno));
		flog(LOG_DEBUG2,"Retrying with not creation\n");
		shm_key[area]=ftok("/tmp",area);
		flog(LOG_DEBUG2,"Shared Memeory Key is : %LX",shm_key[area]);
		if((v_shm_id[area]=shmget(shm_key[area],total_memeory_size,SHM_NORESERVE|IPC_CREAT|PERM)<0)) {
			flog(LOG_DEBUG2,"Returning from shmget errno is %d error %s \n",errno,strerror(errno));
			return(-1);
		} else {
			init_b_flag=0;
		}

	} else {
			init_b_flag=1;
	}
	flog(LOG_INFO,"Attacching shared memory area id := %d\n",v_shm_id[area]);
	if((v_shm_ptrs[area]=(t_block*) shmat(v_shm_id[area],ptr,SHM_REMAP|SHM_RND))==(t_block *) -1) {
		errn=errno;
		flog(LOG_DEBUG1,"Error attaching Shared Memory");
		flog(LOG_DEBUG2,"Error is %s\n",strerror(errn));
		return (-1);
	} else {
		if(mlock(v_shm_ptrs[area],total_memeory_size)<0) {
			flog(LOG_DEBUG2,"Error locking memory area \"%10p\" %d size ( %s )\n",v_shm_ptrs[area],total_memeory_size,strerror(errno));
		}
		c=0;
		shmptrtmp=v_shm_ptrs[area];
		flog(LOG_DEBUG3,"Memory Mapping for area %d is %10p\n",area,v_shm_ptrs[area]);
		while (c<block_number && init_b_flag==1) {
			flog(LOG_INFO,"Resetting all bloxks");
			init_blk(shmptrtmp,block_size,c);
			flog(LOG_DEBUG4,"Block %d dump\n",c);
			flog(LOG_DEBUG4,"Block size :%lu\n",(unsigned long )block_size);
			flog(LOG_DEBUG4,"Block heder size :%lu \n",(unsigned long )(sizeof(shmptrtmp->block_header)));
			flog(LOG_DEBUG4,"Block data buffer size:%lu\n",(unsigned long )((shmptrtmp->block_header.blk_size)-(sizeof(shmptrtmp->block_header))));
			flog(LOG_DEBUG4,"Block pointer %20p\n",(long long)shmptrtmp);
			flog(LOG_DEBUG4,"Block pointer header %20p\n",(long long)&(shmptrtmp->block_header));
			flog(LOG_DEBUG4,"Block pointer buffer %20p\n",(long long)(shmptrtmp->buffer));
			shmptrtmp= (t_block *) ((unsigned char *) shmptrtmp + blk_area_size[area]);
			c++;
		}
		if(init_b_flag==0)
			flog(LOG_DEBUG2,"Shared Memeory allready exists not initializing them\n");
		else
			flog(LOG_DEBUG2,"Shared Memory Blocks initialized\n");
	}

	return(total_memeory_size);
}

