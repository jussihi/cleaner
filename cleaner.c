#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include "cleanerlib.h"
#include "hhlib.h"

void sig_term(int signal)
{
    switch (signal)
    {
    case SIGTERM:
        printf("%d: Caught SIGTERM, exiting now\n", getpid());
        hhrelease();
        printf("exit.\n");
        exit(0);
    default:
        fprintf(stderr, "Caught wrong signal: %d\n", signal);
        return;
    }
    printf("Signal %d caught\n", signal);
}


int main(int argc, char** argv)
{
    
    struct sigaction sa;

    sigemptyset(&sa.sa_mask);

    sa.sa_handler = &sig_term;

    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGINT, &sa, NULL);

    if(argc != 2)
    {
        printf("usage: %s filename\n", argv[0]);
        return -1;
    }

    sleep(10);

    // joku kirjasto jolle annetaan tasan 1 argumentti,
    // tällöin kyseistä voidaan kutsua execillä monta kertaa

    printf("Cleaner %d called with name %s!\n", getpid(), argv[1]);

    //sleep(20);

    remove_comments(argv[1]);
    printf("Cleaner %d removed comments, sleeping 5sec!\n", getpid());
    //sleep(20);
    indent_code(argv[1], "  ");
    printf("Cleaner %d indented code comments, sleeping 5sec!\n", getpid());
    //sleep(20);

    char* delName = calloc(strlen(argv[1]) + 5, 1);
    strcpy(delName, argv[1]);
    strcat(delName, ".rem");
    remove(delName);

    free(delName);

	return 0;
}