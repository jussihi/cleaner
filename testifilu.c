#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int cleaner(const char* filename)
{

    FILE* inFile  = fopen(filename, "r");

    // read failed
    if(!inFile)
    {
        printf("opening failed\n");
        return -1;
    }

    char* outName = calloc(strlen(filename) + 7, 1);
    /*
     * Toisenlainen testikommentti
     */
    

    strcpy(outName, filename);
    strcat(outName, ".clean");

    FILE* outFile = fopen(outName, "w");

    // read failed
    if(!outFile)
    {
        printf("write open failed\n");
        return -1;
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
        switch(rs)
        {
            case CODE:
            {
                if (inchar == '/' && nextchar == '/')
                {
                    rs = LINECOMMENT;
                    // input++;
                }
                else if (inchar == '/' && nextchar == '*')
                {
                    rs = BLOCKCOMMENT;
                    // input++;
                }
                else
                {
                    // lisää outputtiin inchar
                    // *op++ = *input;
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
                    // input++;
                    rs = CODE;
                }
                break;
            }
        }
        inchar = nextchar;
    }


    if(ferror(inFile))
    {
        perror("ferror: Error occured\n");
        return -1;
    }

    // release heap
    free(outName);

    // close file pointers
    fclose(inFile);
    fclose(outFile);

    return 0;
}

int main(void)
{
    printf("asddd\n");
    cleaner("testifilu.c");
    return 0;
}