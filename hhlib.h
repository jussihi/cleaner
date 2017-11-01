/*
 * HEAPHANDLER - A simple C garbage collector made by Jussi Hietanen
 * 
 * This is a library that helps handling the heap allocations in case
 * of premature exit of a program
 */


#include <sys/types.h> // mode_t

#ifndef HH_LIB_C
#define HH_LIB_C

/*
 * Heap handler struct. 
 * Stores the heap arrays
 */
typedef struct {
    void** hpArr;
    FILE** fpArr;
    int* fdArr;
    int hpNum;
    int fpNum;
    int fdNum;
} hhStruct;

/*
 * Global heap_handler is initialized in the hhlib.c source file
 */
extern hhStruct heap_handler;

/*
 * Function definitions for the heaphandler library
 * These functions are wrappers for the C standard
 * commands, for example a wrapper for malloc() is
 * called hhmalloc() here. The usage and interface is 
 * the same as with malloc(). The wrapper functions just 
 * have a "hh" prefix.
 * 
 * This applies to all of the wrapper functions 
 * in this library
 */

void* hhmalloc(size_t size);

void* hhcalloc(size_t nmemb, size_t size);

void hhfree(void* ptr);

/*
 * The only method which is not a wrapper to a standard
 * function. This function simply releases the heap.
 * This could (and should) be called just before exiting the program.
 */
void hhrelease();

int hhopen(const char *pathname, int flags, mode_t mode);

void hhclose(int fd);

void hhclose(int fd);

FILE* hhfopen(const char *pathname, const char *mode);

void hhfclose(FILE* stream);

#endif