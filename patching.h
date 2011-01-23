#ifndef PATCHING_H
#define PATCHING_H

#ifdef __cplusplus
extern "C" {
#endif

extern inline int mem8eq(const void * addr, const void* data) {
        unsigned int *p1 = (unsigned int *) addr;
        unsigned int *p2 = (unsigned int *) data;
        return (
		*(p1++)==*(p2++)
		&& *p1==*p2
		);
}

extern inline int mem16eq(const void * addr, const void* data) {
        unsigned int *p1 = (unsigned int *) addr;
        unsigned int *p2 = (unsigned int *) data;
        return (
		*(p1++)==*(p2++)
		&& *(p1++)==*(p2++)
		&& *(p1++)==*(p2++)
		&& *p1==*p2
		);
}

int _memcmp(const void *s1, const void *s2, size_t n);
void* _memcpy(void* dest, const void* src, size_t n);
void* memfind(const void* data, unsigned int size, const void* search, unsigned int n);
void* memfind4(const void* data, unsigned int size, unsigned int search);
void* memfind8(const void* data, unsigned int size, const void* search);
void* memfind16(const void* data, unsigned int size, const void* search);
void* memfindref8(const void* data, unsigned int size, const void* search);
void dump(void *addr, unsigned int size);

#ifdef __cplusplus
}
#endif

#endif
