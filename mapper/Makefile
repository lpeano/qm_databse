CC=gcc
CFLAGS= -c -Wall -Werror -fPIC  
SHLIB=  -shared -o 

basepath=/home/lpeano/qm_databse
local_path=mapper

BitMap.o_include=log $(basename)/$(local_path)
BitMap_link_dir=log
BitMap_link=flog 

link_flags_path=$(foreach dir,$($@_link_dir), -L$(basepath)/$(dir))
#link_flags=$(foreach dir,$($@_link), -l$(dir))
link_flags=$(foreach dir,$($(basename $@)_link), -l$(dir))
include_flags=$(foreach dir,$($@_include), -I$(basepath)/$(dir))


all: clean BitMap.o BitMap.so 

BitMap.so: BitMap.o
	$(CC) $(SHLIB) lib$@ $(basename $@).o

BitMap.o:
	$(CC) $(CFLAGS)	$(basename $@).c $(include_flags) $(link_flags)  -o $@

clean: 
	rm -f *.o *.so

install: all 
	mkdir -p $(basepath)/lib
	cp *.so $(basepath)/lib	
	
	

