#ifndef HH_LOG_C
#define HH_LOG_C 

/*
 * Writes log entries into file descriptor.
 * This could've been done with same sort 
 * of extern variable but had no time any
 * more.
 */

void log_write(int fd, int count, ...);

#endif