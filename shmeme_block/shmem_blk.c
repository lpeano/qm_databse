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
	unsigned long c=0;
	U_1024_BYTE *k_ptr=(U_1024_BYTE*) NULL;

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
	flog(LOG_DEBUG4,"From byte %20p to byte %20p --> %20p",blk->block_header.Chunk_BitMap,&(blk->block_header.Chunk_BitMap[7]),(&(blk->block_header.Chunk_BitMap[7].byte[1023])));
	flog(LOG_DEBUG4,"Size of Chunk_BitMap is %lu bytes equivalent to %lu bit",sizeof(blk->block_header.Chunk_BitMap),sizeof(blk->block_header.Chunk_BitMap)*8);
	for(c=0;c<sizeof(blk->block_header.Chunk_BitMap)*8;c++) {
		k_ptr=blk->block_header.Chunk_BitMap;
		flog(LOG_DEBUG4,"Bitmap counter %lu Bitmap offset %lu pointer to %20p",c,GET_INDEX_BITMAP((c)),(U_1024_BYTE*)(&(blk->block_header.Chunk_BitMap[GET_INDEX_BITMAP((c))])));
		flog(LOG_DEBUG4,"Bitmap counter %lu Bitmap offset %lu pointer to %20p",c,GET_INDEX_BITMAP((c)),((U_1024_BYTE*)(k_ptr)+c/8192));
		//unset_bit((U_1024_BYTE*)(&(blk->block_header.Chunk_BitMap[GET_INDEX_BITMAP((c))])),(c));
		  unset_bit((U_1024_BYTE*)((blk->block_header.Chunk_BitMap)),(c));
	}
	for(c=0;c<sizeof(blk->block_header.Chunk_Record_BitMap)*8;c++) {
		flog(LOG_DEBUG4,"Bitmap counter %lu Bitmap offset %lu",c,GET_INDEX_BITMAP((c)));
		unset_bit((U_1024_BYTE*)(&(blk->block_header.Chunk_Record_BitMap)),(c));
	}
	for( c=block_size-1 ;c>blk->block_header.free_space-1;c--) {
		flog(LOG_DEBUG4,"Bitmap counter %lu Bitmap offset %lu",c,GET_INDEX_BITMAP((c)));
		  set_bit((U_1024_BYTE*)(&(blk->block_header.Chunk_BitMap)),c);
	}

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
	int new_chunk=0;
	unsigned long bit;
	U_1024_BYTE *BitMap_ptr=NULL;

	BitMap_ptr=(U_1024_BYTE *) blk->block_header.Chunk_BitMap;
	unsigned long c=0,number_of_bytes=0;
	CH *last;
	for (c=0;(c<BLOCK_BITMAP_BITS_SIZE) && (number_of_bytes<buffer_size);c++) {
		/* Locking for minimum chunks bytes  */
		long n=0;
		flog(LOG_DEBUG4,"Starting searching reclaimed %lu bytes from bitmap pointer %lu of %lu",buffer_size,c,BLOCK_BITMAP_BITS_SIZE);
		flog(LOG_DEBUG4,"Minimum chunk size is %lu c is %lu n is %ld",(unsigned long)sizeof(t_chunk),c,n);
		for(n=0;n<chun_minimum_size;n++,c++) {
			bit=GET_BLK_BIT((unsigned char*)BitMap_ptr,c);	
			bit=get_bit(BitMap_ptr,c);
			flog(LOG_DEBUG4,"Getting header bit number c:%lu n:%ld=> %lu for chunk",c,n,bit);
			if(bit==1) {
				n=0;
				break;
			}
		}
		if (n>0) {
			/* Getted minimum chunk size data
			 * now continuing after allocating
			 */ 
			flog(LOG_DEBUG4,"Matched minimum chunk size at bit absolute pointer %lu and relativa pointer %lu",c,n);
			/* Variable bitmap allcoator */
			if(new_chunk==1) { 
				flog(LOG_DEBUG4,"Adding new CH structure\n\t\t\tstart:%lu initial size:%lu",c-n,n);
				add_ch( last->ch_number+1,last->blk_id, 0, 0 ); 
				last=(CH *) get_last();
				last->start=c-n;
				last->size=n;
				new_chunk=0;
			} else {
				
				last=(CH *) get_last();
				if( last==(CH *) NULL) {
					flog(LOG_ERROR,"Last ch pointer is NULL !!! Somthinkg goes bad!!!");
				}			
				flog(LOG_DEBUG4,"Starting pointer is c-n =%lu - %ld = %ld",c,n,c-n);
				last->start=c-n;	/* Set first pointer in buffer */
				last->size=n;		/* Set actual size in buffer   */
				number_of_bytes=1;
			}
			//c++;
			/*
			 * Now allocating remaining values
			 *
			 */
			long m=c;
			for(m=c;(number_of_bytes<(buffer_size)) && m<BLOCK_BITMAP_BITS_SIZE ;c++,m++) {
				bit=GET_BLK_BIT((unsigned char*)BitMap_ptr,m);	
				bit=get_bit(BitMap_ptr,m);
				flog(LOG_DEBUG4,"Getting %lu on BitMap value is %lu number of bytes %lu max number of bytes %lu",m,bit,number_of_bytes,buffer_size);
				if(bit==1) {
					n=-1;
					/* Add new chunk elment */
					//add_ch( last->ch_number+1,last->blk_id, 0, 0 ); 
					new_chunk=1;
					flog(LOG_DEBUG4,"After bit number %lu no chunk founded restart finding creating new chunk",c);
					break;
				} else {
					flog(LOG_DEBUG4,"Getting new byte at offset %lu for buffer_size:%lu number_of_bytes:%lu",c,buffer_size,number_of_bytes);
					last->size++; 
					number_of_bytes++;
				}
			}	

		} else {
			flog(LOG_DEBUG4,"After bit number %lu no chunk founded",c);
		}
	}
	return(number_of_bytes);
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
	CH *tmp_ch=(CH*) NULL;


	block=(t_block *)GET_BLOCK(bcount,area,block_size);
	flog(LOG_DEBUG4,"Block number %lu  of size %10lu is pointed by %p",bcount,block_size,(char *) block);
	flog(LOG_DEBUG4,"Free space in block : %llu",block->block_header.free_space);
	flog(LOG_DEBUG4,"Sixe of chunk header is %lu",sizeof(t_chunk));
	if( (space+sizeof(t_chunk)) <= block->block_header.free_space ) {
		flog(LOG_DEBUG4,"Block space is appropriate to allocate %lu bytes total free space is %lu bytes",space,block->block_header.free_space);	
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
			tmp_ch=CH_PTR;
				flog(LOG_DEBUG4,"Ptr to bitmap start:%20p end:%20p",&block->block_header.Chunk_BitMap[0],
						&(&block->block_header.Chunk_BitMap[7])->byte[1023]);
				flog(LOG_DEBUG4,"Ptr to bitmap start:%20p end:%20p Ptr %20p",&block->block_header.Chunk_Record_BitMap[0],
						&(&block->block_header.Chunk_Record_BitMap[7])->byte[1023],
						(U_1024_BYTE*)(&block->block_header.Chunk_Record_BitMap[GET_INDEX_BITMAP(0)]));
			set_bit((U_1024_BYTE*)(&(block->block_header.Chunk_BitMap)),CH_PTR->start);
			flog(LOG_DEBUG4,"Settimg Record Bitmap");
			set_bit((U_1024_BYTE*)(&(block->block_header.Chunk_Record_BitMap)),CH_PTR->start);
			//block->block_header.free_space-=sizeof(t_chunk);
			do{
				unsigned long c;
				flog(LOG_DEBUG4,"Setting BitMap for %lu bytes chunk n° %lu",tmp_ch->size,tmp_ch->ch_number);
				for(c=0;c<tmp_ch->size;c++) {
					if(block->block_header.free_space==0) {
						flog(LOG_ERROR,"BUG on space counter counting is :%lu",c);
					}
					flog(LOG_DEBUG4,"BitMap offset %lu BitMap place %lu free space size %ld",GET_INDEX_BITMAP((tmp_ch->start+c)),tmp_ch->start+c,block->block_header.free_space);
					set_bit((U_1024_BYTE*)(&(block->block_header.Chunk_BitMap)),(tmp_ch->start+c));
					block->block_header.free_space--;
				}
				int chn=0;
				/* Copy data tu buffer */
				flog(LOG_DEBUG4,"All bitmap activated allocating values of %lu bytes",tmp_ch->size-sizeof(t_chunk));
				chunk_ptr=(t_chunk*)((char*)block->buffer+tmp_ch->start+chn);
				chunk_ptr->chunk_id=tmp_ch->ch_number;
				chunk_ptr->chunk_size=tmp_ch->size;
				chunk_ptr->next=-1;
				flog(LOG_DEBUG4,"Pointer to chunk structure is %20p",chunk_ptr);
				flog(LOG_DEBUG4,"Pointer to data buffer is     %20p",chunk_ptr->data_buffer);
				for(chn=0;chn<(tmp_ch->size)-sizeof(t_chunk);chn++) {
					*((char *) chunk_ptr->data_buffer+chn)=*((char *) buffer+chn);
					flog(LOG_DEBUG4,"Allocating byte %llu",(unsigned long long) (chunk_ptr->data_buffer+chn));
				}
				if(tmp_ch->next!=(CH*) NULL) { 
					chunk_ptr->next=(tmp_ch->next)->start;
				}
			} while (tmp_ch->next!=(CH*) NULL);
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
	char *last=(char *)0x40001000;
	char *last1;
	struct shmid_ds shmids;

	t_block *shmptrtmp;
	flog(LOG_INFO,"Memory Mapping for:\n\t etext :%10p\n\t edata :%10p\n\t eend :%10p",&etext,&edata,&end);
	/*if ( brk(last)<0) {
		flog(LOG_INFO,"Error moving heap break to %20p \"%s\" ",last,strerror(errno));
	}*/
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
	} else {
			shmctl(v_shm_id[area],IPC_STAT,&shmids);
			if(getpid()==(pid_t)shmids.shm_cpid) {
				flog(LOG_DEBUG2,"Process pid is %d shm creator pid is %d I am creator",getpid(),shmids.shm_cpid);
				init_b_flag=1;
			} else {
				flog(LOG_DEBUG2,"Process pid is %d shm creator pid is %d",getpid(),shmids.shm_cpid);
				init_b_flag=0;
			}

	}
	flog(LOG_INFO,"Attacching shared memory area id := %d\n",v_shm_id[area]);
	if((v_shm_ptrs[area]=(t_block*) shmat(v_shm_id[area],ptr,SHM_REMAP|SHM_RND))==(t_block *) -1) {
		errn=errno;
		flog(LOG_DEBUG1,"Error attaching Shared Memory");
		flog(LOG_DEBUG2,"Error is %s\n",strerror(errn));
		return (-1);
	} else {
		/*
		if(mlock(v_shm_ptrs[area],total_memeory_size)<0) {
			flog(LOG_DEBUG2,"Error locking memory area \"%10p\" %d size ( %s )\n",v_shm_ptrs[area],total_memeory_size,strerror(errno));
		}*/
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

