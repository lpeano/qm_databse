CC=gcc
CFLAGS= -c -Wall -Werror -fPIC  
SHLIB=  -shared -o 

basepath=/home/lpeano/develompent
local_path=log

flog.o_include=log $(basename)/$(local_path)
flog_link_dir=log
flog_link=flog

link_flags_path=$(foreach dir,$($@_link_dir), -L$(basepath)/$(dir))
link_flags=$(foreach dir,$($(basename $@)_link), -l$(dir))
include_flags=$(foreach dir,$($@_include), -I$(basepath)/$(dir))


all: clean flog.o flog.so 

flog.so: flog.o
	$(CC) $(SHLIB) lib$@ $(basename $@).o

flog.o:
	$(CC) $(CFLAGS)	$(basename $@).c $(include_flags) $(link_flags)  -o $@

clean: 
	rm -f *.o *.so

install: all 
	mkdir -p $(basepath)/lib
	cp *.so $(basepath)/lib	

#LocalPath=/home/lpeano/develompent/log
#CC=gcc
#CFLAGS= -c -Wall -Werror -fPIC
#SHLIB=  -shared -o 
#
#all: clean flog.o libflog.so 
#
#libflog.so: flog.o
#	$(CC) $(SHLIB) $(LocalPath)/libflog.so $(LocalPath)/flog.o
#
#flog.o:
#	$(CC) $(CFLAGS)	$(LocalPath)/flog.c -o $(LocalPath)/flog.o
#
#clean: 
#	rm -f $(LocalPath)/*.o $(LocalPath)/*.so
