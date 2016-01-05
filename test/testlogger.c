#include <flog.h>

int main(int ergc, char *argv[]) {
char log_file[1000]="prova.log\0";
init_log_level(LOG_INFO);
init_logger((char *)log_file); 
flog(LOG_INFO,"prova 1");
flog(LOG_DEBUG1,"prova 2");
init_log_level(LOG_DEBUG1);
flog(LOG_DEBUG1,"prova 3");
init_log_level(LOG_DEBUG2);
flog(LOG_DEBUG1,"prova 4");
flog(LOG_DEBUG2,"prova 5");

return(0);
}
