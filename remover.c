#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "hhlib.h"


int remove_comments(const char* filename)
{
    FILE* inFile  = hhfopen(filename, "r");

    // read failed
    if(!inFile)
    {
        return -1;
    }

    char* outName = hhcalloc(strlen(filename) + 5, 1);

    strcpy(outName, filename);
    strcat(outName, ".rem");

    FILE* outFile = hhfopen(outName, "w");

    hhfree(outName);

    // read failed
    if(!outFile)
    {
        return -2;
    }

    typedef enum
    {
        CODE,
        LINECOMMENT,
        BLOCKCOMMENT
    } Readstatus;
 
    Readstatus rs = CODE;

    char inchar, nextchar;

    inchar = fgetc(inFile);

    while (inchar != EOF)
    {
        nextchar = fgetc(inFile);
        while(inchar == '\n' && nextchar == '\n')
        {
            nextchar = fgetc(inFile);
        }
        switch(rs)
        {
            case CODE:
            {
                if (inchar == '/' && nextchar == '/')
                {
                    rs = LINECOMMENT;
                }
                else if (inchar == '/' && nextchar == '*')
                {
                    rs = BLOCKCOMMENT;
                }
                else
                {
                    fputc(inchar, outFile);
                }
                break;
            }
            case LINECOMMENT:
            {
                if (inchar == '\n')
                {
                    rs = CODE;
                }
                break;
            }
                
                
            case BLOCKCOMMENT:
            {
                if (inchar == '*' && nextchar == '/')
                {
                    rs = CODE;
                    nextchar = fgetc(inFile);
                }
                break;
            }
        }
        inchar = nextchar;
    }

    if(ferror(inFile))
    {
        perror("ferror: Error occured\n");
        return -3;
    }

    // close file pointers
    hhfclose(inFile);
    hhfclose(outFile);

    return 0;
}
