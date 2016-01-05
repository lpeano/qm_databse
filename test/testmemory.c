#include <shmem_blk.h>
#include <flog.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
int main(int ergc, char *argv[]) {
char Buffer_Log[1000];
init_log_level(LOG_DEBUG4);
memset(Buffer_Log,0,sizeof(Buffer_Log));
//sprintf(Buffer_Log,"Memory allocation of %d blocks is %d bytes\n",1000, (int) init_shmem_block(0,1000,1024*8));
//sprintf(Buffer_Log,"Memory allocation of %d blocks is %d bytes\n",1000, (int) init_shmem_block(1,1000,1024*64));
flog(LOG_INFO,"Memory allocation of %d blocks is %d bytes\n",1000, (int) init_shmem_block(2,100,1024));
while(1)sleep(1);
return(0);
}
