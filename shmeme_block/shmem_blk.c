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

int init_blk( t_block *blk ,size_t block_size,int block_id)
{
	/* To be implemented */
	blk->block_header.blk_size=block_size;
	blk->block_header.blk_id=block_id;
	memset((char *) blk->buffer,0,block_size);
	return(0);
}
/*
 * Definse 1000 possible size of shm blocks buffers
 */
size_t blk_area_size[1000];

/*
 * Templete memory allocation.
 *
 *
 *
 *
 */
size_t init_shmem_block( int area, int block_number, size_t block_size ) {
	int c,errn=0,init_b_flag;
	size_t total_memeory_size=0;
	t_block *shmptrtmp;
	blk_area_size[area]=block_size;
	total_memeory_size=block_number*block_size+(sizeof(t_block)*block_number);
	shm_key[area]=ftok("/tmp",area);
	if((v_shm_id[area]=shmget(shm_key[area],total_memeory_size,IPC_CREAT|PERM))<0) {
		flog(LOG_DEBUG2,"Returning from shmget errno is %d error %s \n",errno,strerror(errno));
		flog(LOG_DEBUG2,"Retrying with not creation\n");
		if((v_shm_id[area]=shmget(shm_key[area],total_memeory_size,PERM)<=0)) {
			flog(LOG_DEBUG2,"Returning from shmget errno is %d error %s \n",errno,strerror(errno));
			return(-1);
		} else {
			init_b_flag=0;
		}

	} else {
			init_b_flag=1;
	}
	flog(LOG_INFO,"Attacching shared memory area id := %d\n",v_shm_id[area]);
	if((v_shm_ptrs[area]=(t_block*) shmat(v_shm_id[area],NULL,SHM_RND))==(t_block *) -1) {
		errn=errno;
		flog(LOG_DEBUG1,"Error attaching Shared Memory");
		flog(LOG_DEBUG2,"Error is %s\n",strerror(errn));
		return (-1);
	} else {
		c=0;
		shmptrtmp=v_shm_ptrs[area];
		while (c<block_number && init_b_flag==1) {
			shmptrtmp->block_header.blk_id=c;	
			shmptrtmp->block_header.blk_size=block_size;
			memset((char*)shmptrtmp->buffer,0,block_size-sizeof(t_block_header));
			flog(LOG_DEBUG4,"Block %d dump\n",c);
			flog(LOG_DEBUG4,"Block size :%d\n",block_size);
			flog(LOG_DEBUG4,"Block pointer %Ld\n",(long long)shmptrtmp);
			flog(LOG_DEBUG4,"Block pointer header %Ld\n",(long long)&(shmptrtmp->block_header));
			flog(LOG_DEBUG4,"Block pointer buffer %Ld\n",(long long)(shmptrtmp->buffer));
			shmptrtmp= (t_block *) ((char *) shmptrtmp + blk_area_size[area]);
			c++;
		}
	}

	return(total_memeory_size);
}
