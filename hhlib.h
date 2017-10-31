#include <sys/types.h> // mode_t

#ifndef HH_LIB_C
#define HH_LIB_C

typedef struct {
    void** hpArr;
    FILE** fpArr;
    int* fdArr;
    int hpNum;
    int fpNum;
    int fdNum;
} hhStruct;

extern hhStruct heap_handler;

void* hhmalloc(size_t size);

void* hhcalloc(size_t nmemb, size_t size);

void hhfree(void* ptr);

void hhrelease();

int hhopen(const char *pathname, int flags, mode_t mode);

void hhclose(int fd);

void hhclose(int fd);

FILE* hhfopen(const char *pathname, const char *mode);

void hhfclose(FILE* stream);

#endif