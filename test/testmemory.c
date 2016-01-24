#include <shmem_blk.h>
#include <flog.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
int main(int ergc, char *argv[]) {
init_logger("/home/lpeano/qm_databse/testmemory.log");	
init_log_level(atoi(argv[2]));
//sprintf(Buffer_Log,"Memory allocation of %d blocks is %d bytes\n",1000, (int) init_shmem_block(0,1000,1024*8));
//sprintf(Buffer_Log,"Memory allocation of %d blocks is %d bytes\n",1000, (int) init_shmem_block(1,1000,1024*64));
flog(LOG_INFO,"Memory allocation of %d blocks is %d bytes\n",1000, (int) init_shmem_block(0,1,64*1024));
flog(LOG_INFO,"Reclaming 1 bytes of memory\n");
//char buffer[128];
char *buffer;
buffer = (char * ) malloc(atoi(argv[1]));
reclaim_free_block_space(0,0,atoi(argv[1]),64*1024,buffer);
flog(LOG_INFO,"Reclaimed bytes");

while(1)sleep(1);
return(0);
}
