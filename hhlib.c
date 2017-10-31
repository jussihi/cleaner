#include <stdlib.h>
#include <stddef.h>
#include <sys/types.h> // mode_t
#include <stdio.h> // fopen, fclose
#include <fcntl.h> // open
#include <unistd.h> // close
#include "hhlib.h"
// #include "hhlibglobal.h"

hhStruct heap_handler = {NULL, NULL, NULL, 0, 0, 0};

void* hhmalloc(size_t size)
{
	void* retPtr = malloc(size);
	// is the array empty?
	if(!heap_handler.hpArr)
	{
		heap_handler.hpArr = malloc(1 * sizeof(void*));
		heap_handler.hpArr[0] = retPtr;
	}
	// else we just append to the end of the array.
	else
	{
		heap_handler.hpArr = realloc(heap_handler.hpArr, (heap_handler.hpNum + 1) * sizeof(void*));
		heap_handler.hpArr[heap_handler.hpNum] = retPtr;
	}
	heap_handler.hpNum++;
	return retPtr;
}

void* hhcalloc(size_t nmemb, size_t size)
{
	void* retPtr = calloc(nmemb, size);
	// is the array empty?
	if(!heap_handler.hpArr)
	{
		heap_handler.hpArr = malloc(1 * sizeof(void*));
		heap_handler.hpArr[0] = retPtr;
	}
	// else we just append to the end of the array.
	else
	{
		heap_handler.hpArr = realloc(heap_handler.hpArr, (heap_handler.hpNum + 1) * sizeof(void*));
		heap_handler.hpArr[heap_handler.hpNum] = retPtr;
	}
	heap_handler.hpNum++;
	return retPtr;
}

void hhfree(void* ptr)
{
	for(int i = 0; i < heap_handler.hpNum; i++)
	{
		if(heap_handler.hpArr[i] == ptr)
		{
			// special case if the array has only 1 member
			if(heap_handler.hpNum == 1)
			{
				free(ptr);
				free(heap_handler.hpArr);
				heap_handler.hpNum = 0;
			}
			else
			{
				free(ptr);
				heap_handler.hpArr[i] = heap_handler.hpArr[heap_handler.hpNum - 1];
				heap_handler.hpArr = realloc(heap_handler.hpArr, (heap_handler.hpNum - 1) * sizeof(void*));
				heap_handler.hpNum--;
			}
			return;
		}
	}
}

void hhrelease()
{
	// the array should always contain something when it is non-null
	if(heap_handler.hpArr)
	{
		// release every member of hpArr
		// also release hpArr itself
		for(int i = 0; i < heap_handler.hpNum; i++)
		{
			free(heap_handler.hpArr[i]);
		}
		free(heap_handler.hpArr);
		heap_handler.hpNum = 0;
	}

	if(heap_handler.fpArr)
	{
		// close every file of fpArr
		// also release fpArr itself
		for(int i = 0; i < heap_handler.fpNum; i++)
		{
			fclose(heap_handler.fpArr[i]);
		}
		free(heap_handler.fpArr);
		heap_handler.fpNum = 0;
	}

	if(heap_handler.fdArr)
	{
		// close every fdArr file descriptor
		// release array from heap as well
		for(int i = 0; i < heap_handler.fdNum; i++)
		{
			close(heap_handler.fdArr[i]);
		}
		free(heap_handler.fdArr);
		heap_handler.fdNum = 0;
	}
}

/*
 * This is an ugly implementation of file descriptor interface
 * This only works with the 3-parameter option of the syscall
 */
int hhopen(const char *pathname, int flags, mode_t mode)
{
	int fd = open(pathname, flags, mode);
	heap_handler.fdArr = realloc(heap_handler.fdArr, (heap_handler.fdNum + 1) * sizeof(int));
	heap_handler.fdArr[heap_handler.fdNum] = fd;
	heap_handler.fdNum++;

	return fd;
}

void hhclose(int fd)
{
	// remove fd from the fdArr
	for(int i = 0; i < heap_handler.fdNum; i++)
	{
		if(heap_handler.fdArr[i] == fd)
		{
			close(fd);
			heap_handler.fdArr[i] = heap_handler.fdArr[heap_handler.fdNum - 1];
			heap_handler.fdArr = realloc(heap_handler.fdArr, (heap_handler.fdNum - 1) * sizeof(int));
			heap_handler.fdNum--;
			return;
		}
	}
	return;
}

/*
 * This is also an ugly implementation of file pointer interface
 */

FILE* hhfopen(const char* pathname, const char* mode)
{
	FILE* retPtr = fopen(pathname, mode);

	// sanity check
	if(!retPtr) return NULL;
	
	// is the array empty?
	if(!heap_handler.fpArr)
	{
		heap_handler.fpArr = malloc(1 * sizeof(FILE*));
		heap_handler.fpArr[0] = retPtr;
	}
	// else we just append to the end of the array.
	else
	{
		heap_handler.fpArr = realloc(heap_handler.fpArr, (heap_handler.fpNum + 1) * sizeof(FILE*));
		heap_handler.fpArr[heap_handler.fpNum] = retPtr;
	}
	heap_handler.fpNum++;
	return retPtr;
}

void hhfclose(FILE* stream)
{
	for(int i = 0; i < heap_handler.fpNum; i++)
	{
		if(heap_handler.fpArr[i] == stream)
		{
			// special case if the array has only 1 member
			if(heap_handler.fpNum == 1)
			{
				fclose(stream);
				free(heap_handler.fpArr);
				heap_handler.fpNum = 0;
			}
			else
			{
				fclose(stream);
				heap_handler.fpArr[i] = heap_handler.fpArr[heap_handler.fpNum - 1];
				heap_handler.fpArr = realloc(heap_handler.fpArr, (heap_handler.fpNum - 1) * sizeof(FILE*));
				heap_handler.fpNum--;
			}
			return;
		}
	}
}