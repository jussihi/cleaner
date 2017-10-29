#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cleanerlib.h"

int main(int argc, char** argv)
{

    if(argc != 2)
    {
        printf("usage: %s filename\n", argv[0]);
        return -1;
    }

    // joku kirjasto jolle annetaan tasan 1 argumentti,
    // tällöin kyseistä voidaan kutsua execillä monta kertaa

    printf("Cleaner called with name %s!\n", argv[1]);

    remove_comments(argv[1]);
    indent_code(argv[1], "  ");

    char* delName = calloc(strlen(argv[1]) + 5, 1);
    strcpy(delName, argv[1]);
    strcat(delName, ".rem");

    remove(delName);

    free(delName);

	return 0;
}