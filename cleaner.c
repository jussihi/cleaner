#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "cleanerlib.h"
#include "hhlib.h"
#include "log.h"

static int log_fd;

void sig_term(int signal)
{
    switch (signal)
    {
    // only handle SIGTERM, exit program if SIGTERM was received
    case SIGTERM:
        log_write(log_fd, 1, "Caught SIGTERM, exiting now.");
        // release heap with the cool carbage collector
        hhrelease();
        exit(0);
    default:
        return;
    }
    printf("Signal %d caught\n", signal);
}


int main(int argc, char** argv)
{
    // open log
    log_fd = open("cleaner.log", O_RDWR | O_APPEND | O_CREAT | O_NONBLOCK, S_IRWXU);

    struct sigaction sa;

    sigemptyset(&sa.sa_mask);

    sa.sa_handler = &sig_term;
    sa.sa_flags = 0;

    // the sig_term only handles SIGTERM and SIGINT
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGINT, &sa, NULL);

    // print error and returnif the program is not being run with appropriate parameters
    if(argc != 2)
    {
        printf("usage: %s filename\n", argv[0]);
        log_write(log_fd, 1, "The cleaner was run without any parameters. Exiting.");
        return -1;
    }

    log_write(log_fd, 2, "Cleaner init with name ", argv[1]);

    int ret;
    // call functions from the cleaner library
    if((ret = remove_comments(argv[1])) != 0)
    {
        char num[3] = { 0 };
        sprintf(num, "%d", ret);
        log_write(log_fd, 2, "ERROR: remove_comments returned error code ", num);
        log_write(log_fd, 1, "Fatal error. Exiting cleaner.");
        return -1;
    }

    log_write(log_fd, 3, "Comments removed. Indenting temporary file ", argv[1], ".rem");
    
    //sleep(20);
    if((ret = indent_code(argv[1], "  ")) != 0)
    {
        printf("asd");
        char num[3] = { 0 };
        sprintf(num, "%d", ret);
        log_write(log_fd, 2, "ERROR: indent_code returned error code ", num);
        log_write(log_fd, 1, "Fatal error. Exiting cleaner.");
        return -1;
    }
    // sleep(20);

    log_write(log_fd, 3, "Cleaning complete. Removing temporary file and exiting. Cleaned file is ", argv[1], ".clean");
    // remove the temporary file with ".rem" file extension
    char* delName = hhcalloc(strlen(argv[1]) + 5, 1);
    strcpy(delName, argv[1]);
    strcat(delName, ".rem");
    remove(delName);
    hhfree(delName);

	return 0;
}