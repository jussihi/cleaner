#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "hhlib.h"

static int childs;

void sig_int(int signal)
{
    printf("moi\n");
    switch(signal)
    {
        case SIGINT:
        {
            printf("moi");
            pid_t p;

            int status;

            kill(0, SIGTERM);

            sleep(1);

            p = waitpid(-1, &status, WNOHANG);
            if(!WIFEXITED(status))
            {
                printf("p: %d\n", p);
                while((p = waitpid(-1, NULL, WNOHANG)) != -1)
                {
                    kill(p, SIGKILL);
                }
            }
            exit(0);
        }
        default:
            break;
    }
}


int main(int argc, char** argv)
{
    childs = argc-1;
    struct sigaction sa;

    sigemptyset(&sa.sa_mask);

    sa.sa_handler = &sig_int;

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    printf("parent pid: %d\n", getpid());
    if(argc == 1)
    {
        printf("usage: %s filename1 filename 2 ...\n", argv[0]);
        return -1;
    }

    // joku kirjasto jolle annetaan tasan 1 argumentti,
    // tällöin kyseistä voidaan kutsua execillä monta kertaa


    pid_t pid;

    for(int i = 1; i < argc; i++)
    {
        pid = fork();
        if(pid == -1)
        {
            perror("Fork failed!\n");
            return -1;
        }
        // täällä kutsutaan kirjastoa aina yhdellä parametrilla, käytetään exec
        if(pid == 0)
        {
            printf("Calling cleaner with filename: %s\n", argv[i]);

            char* args[] = {"./cleaner", argv[i], NULL};

            execvp(args[0], args);
            
            return 0;
        }
    }
    sleep(20);
    // onko parempi näin vai ylemmän for-loopin sisään elsenä????
    for(int i = 1; i < argc; i++)
    {
        if(pid != 0)
        {
            if(waitpid(-1, NULL, 0) <= 0)
            {
                perror("waitpid failed");
            }
        }
    }
	printf("Every cleaner returned\n");

	return 0;
}