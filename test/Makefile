CC=gcc
basepath=/home/lpeano/qm_databse

BitMap.o_include=mapper log
BitMap_link_dir=mapper log
BitMap_link=BitMap flog

testlogger.o_include=log
testlogger_link_dir=log
testlogger_link=flog

testmemory.o_include=log shmeme_block mapper
testmemory_link_dir=log shmeme_block mapper
testmemory_link=flog shmem_blk BitMap

link_flags_path=$(foreach dir,$($@_link_dir), -L$(basepath)/$(dir))
link_flags=$(foreach dir,$($@_link), -l$(dir))

include_flags=$(foreach dir,$($@_include), -I$(basepath)/$(dir))
CFLAGS=-c -Wall -Werror  

testmemory: testmemory.o
	$(CC) -o $@ $@.o  $(link_flags) $(link_flags_path)

testmemory.o:
	 $(CC) $(CFLAGS) $(include_flags)  testmemory.c 

testlogger: testlogger.o
	$(CC) -o $@ $@.o  $(link_flags) $(link_flags_path)

testlogger.o:
	 $(CC) $(CFLAGS) $(include_flags)  testlogger.c 

BitMap: BitMap.o
	$(CC) -o $@ $@.o  $(link_flags) $(link_flags_path)

BitMap.o:
	 $(CC) $(CFLAGS) $(include_flags)  BitMap.c 
clean:
	$(RM) *.o testmemory testlogger

install-testmemory: testmemory
	mkdir -p $(basepath)/bin
	cp testmemory $(basepath)/bin
