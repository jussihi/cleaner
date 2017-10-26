#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cleanerlib.h"

int main(int argc, char** argv)
{
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
            printf("Calling cleaner with filename : %s\n", argv[i]);

            remove_comments(argv[i]);
            indent_code(argv[i], "  ");

            char* delName = calloc(strlen(argv[i]) + 5, 1);
            strcpy(delName, argv[i]);
            strcat(delName, ".rem");

            remove(delName);

            return 0;
        }
    }

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