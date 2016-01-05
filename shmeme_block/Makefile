CC=gcc
CFLAGS= -c -Wall -Werror -fPIC  
SHLIB=  -shared -o 

basepath=/home/lpeano/qm_databse
local_path=shmeme_block

shmem_blk.o_include=log $(basename)/$(local_path)
shmem_blk_link_dir=log
shmem_blk_link=flog 

link_flags_path=$(foreach dir,$($@_link_dir), -L$(basepath)/$(dir))
#link_flags=$(foreach dir,$($@_link), -l$(dir))
link_flags=$(foreach dir,$($(basename $@)_link), -l$(dir))
include_flags=$(foreach dir,$($@_include), -I$(basepath)/$(dir))


all: clean shmem_blk.o shmem_blk.so 

shmem_blk.so: shmem_blk.o
	$(CC) $(SHLIB) lib$@ $(basename $@).o

shmem_blk.o:
	$(CC) $(CFLAGS)	$(basename $@).c $(include_flags) $(link_flags)  -o $@

clean: 
	rm -f *.o *.so

install: all 
	mkdir -p $(basepath)/lib
	cp *.so $(basepath)/lib	
	
	
