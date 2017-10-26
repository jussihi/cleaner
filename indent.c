#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void indent_code(const char* filename, const char *pad)
{
    char* inName = calloc(strlen(filename) + 5, 1);

    strcpy(inName, filename);
    strcat(inName, ".rem");

    FILE* inFile  = fopen(inName, "r");

    free(inName);

    // read failed
    if(!inFile)
    {
        printf("opening failed\n");
        return;
    }

    char* outName = calloc(strlen(filename) + 7, 1);

    free(outName);

    strcpy(outName, filename);
    strcat(outName, ".clean");

    FILE* outFile = fopen(outName, "w");

    // read failed
    if(!outFile)
    {
        printf("write open failed\n");
        return;
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

    // close file pointers
    fclose(inFile);
    fclose(outFile);

    return;
}