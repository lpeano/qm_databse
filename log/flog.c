#ifndef _LOGING__
#define _LOGING__
#define _LOG_LIB 1
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <log_levels.h>

#ifndef  _LOG_LEVELS
#define  _LOG_LEVELS

static char *decoded_level[]={
"LOG_INFO\0",
"LOG_LOGVERBOSE\0",
"LOG_DEBUG1\0",
"LOG_DEBUG2\0",
"LOG_DEBUG3\0",
"LOG_DEBUG4\0",
};

int log_level = LOG_INFO;
int fd_log = -1 ;
FILE *stream_log=NULL;
#endif

static char * get_file_time(){
        static char f_t[40];
        time_t now;
        now=time(NULL);
        memset(f_t,'\0',sizeof(f_t));
        strftime(f_t,40,"%Y-%m-%d %H:%M:%S",localtime(&now));
        return (f_t);
}

void flog ( int level, char *message, ...) {
	va_list ap;
	char buffer[1000];

	memset(buffer,0,1000);

	if ( fd_log < 0 ) {
		fd_log = 0;
		stream_log=fdopen(fd_log,"a+");
	}
	if ( level <= log_level ) 
	{
		stream_log=fdopen(fd_log,"a+");
		va_start(ap,message);
		//vfprintf(stream_log,message,ap);
		vsnprintf(buffer,1000,message,ap);
		if( level <0 )
		       	fprintf(stream_log," [ %s ] - [ %s ] : %s\n",get_file_time(),"LOG_ERROR",buffer);
		else
			fprintf(stream_log," [ %s ] - [ %s ] : %s\n",get_file_time(),decoded_level[level-1],buffer);
		va_end(ap);
		fflush(stream_log);

	}
}

int init_log_level( int level ) {
	
	if (level >=LOG_INFO && level <= LOG_DEBUG4) {
		log_level=level;
	} else {
		log_level=LOG_INFO;
		return(1);
	}

	return(0);
}

int init_logger( char *log_file_path ) {
	char lfp[1000];
	memset(lfp,'\0',sizeof(lfp));
	strncpy(lfp,log_file_path,sizeof(lfp));
	if ( strlen(lfp) >0 && strncmp(lfp,"-",sizeof(lfp))!=0 ) {
		if((fd_log=open(lfp,O_APPEND|O_RDWR))<0) {
			perror(sys_errlist[errno]);
			fd_log=0;
			return(1);
		} 
	} 
	return(0);
}

#endif
