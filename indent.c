#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "hhlib.h"

int indent_code(const char* filename, const char *pad)
{
    char* inName = hhcalloc(strlen(filename) + 5, 1);

    strcpy(inName, filename);
    strcat(inName, ".rem");

    FILE* inFile  = hhfopen(inName, "r");

    hhfree(inName);

    // read failed
    if(!inFile)
    {
        return -1;
    }

    char* outName = hhcalloc(strlen(filename) + 7, 1);

    strcpy(outName, filename);
    strcat(outName, ".clean");

    FILE* outFile = hhfopen(outName, "w");
    
    hhfree(outName);

    // read failed
    if(!outFile)
    {
        return -2;
    }
    
    int curtabs = 0;
    int newline = 0;

    char inchar;

    while((inchar = fgetc(inFile)) != EOF)
    {
        if(inchar == '}')
        {
            curtabs--;
        }
        if (curtabs < 0)
        {
            curtabs = 0;
        }
        
        if (newline)
        {
            if (inchar == '\n')
            {
                fputc(inchar, outFile);
                continue;
            }
            if (isspace((int)inchar))
            {
                continue;
            }
            for (int i = 0; i < curtabs; i++)
            {
                fputs(pad, outFile);
            }
            fputc(inchar, outFile);
            newline = 0;
        }
        else
        {
            fputc(inchar, outFile);
            if (inchar == '\n')
            {
                newline = 1;
            }
        }
        if (inchar== '{')
        {
            curtabs++;
        }
    }

    if(ferror(inFile) || ferror(outFile))
    {
        perror("ferror: Error occured\n");
        return -3;
    }

    // close file pointers
    hhfclose(inFile);
    hhfclose(outFile);

    return 0;
}