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

extern inline void* _memset(void* b, int c, size_t n) {
        char* dst8 = (char*)b;
        while (n--) *dst8++ = (unsigned char) c;
        return b;
}

extern inline void* _memcpy(void* dest, const void* src, size_t n) {
        char* dst8 = (char*)dest;
        char* src8 = (char*)src;
        while (n--) *dst8++ = *src8++;
        return dest;
}

extern inline int _memcmp(const void *s1, const void *s2, size_t n) {
	int  v = 0;
	unsigned char *p1 = (unsigned char *) s1;
	unsigned char *p2 = (unsigned char *) s2;
	while (n-- > 0 && v == 0) v = *(p1++) - *(p2++);
	return v;
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
