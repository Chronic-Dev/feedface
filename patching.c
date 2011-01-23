#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "patching.h"

void* _memset(void* b, int c, size_t n) {
        char* dst8 = (char*)b;
        while (n--) *dst8++ = (unsigned char) c;
        return b;
}

void* _memcpy(void* dest, const void* src, size_t n) {
        char* dst8 = (char*)dest;
        char* src8 = (char*)src;
        while (n--) *dst8++ = *src8++;
        return dest;
}

int _memcmp(const void *s1, const void *s2, size_t n) {
	int  v = 0;
	unsigned char *p1 = (unsigned char *) s1;
	unsigned char *p2 = (unsigned char *) s2;
	while (n-- > 0 && v == 0) v = *(p1++) - *(p2++);
	return v;
}

void* memfind(const void* data, unsigned int size, const void* search, unsigned int n) {
        void* ret = NULL;
        unsigned int i = 0;
        unsigned char* pdata = (unsigned char*) data;
        for(i = 0; i < size; i++) {
                if(!_memcmp(pdata, search, n)) {
                        ret = pdata;
                        break;
                }
		pdata++;
        }
        return ret;
}

void* memfind4(const void* data, unsigned int size, unsigned int search) {
        void *ret = NULL;
        unsigned int i = 0;
        unsigned char* pdata = (unsigned char*) data;
	for(i = 0; i < size; i+=2) {
                if(*((unsigned int*) pdata)==search) {
                        ret = (void*) pdata;
                        break;
                }
		pdata+=2;
        }
        return ret;
}

void* memfind8(const void* data, unsigned int size, const void* search) {
        void *ret = NULL;
        unsigned int i = 0;
        unsigned char* pdata = (unsigned char*) data;
	for(i = 0; i < size; i+=2) {
                if(mem8eq(pdata, search)) {
                        ret = (void*) pdata;
                        break;
                }
		pdata+=2;
        }
        return ret;
}

void* memfind16(const void* data, unsigned int size, const void* search) {
        void *ret = NULL;
        unsigned int i = 0;
        unsigned char* pdata = (unsigned char*) data;
	for(i = 0; i < size; i+=2) {
                if(mem16eq(pdata, search)) {
                        ret = (void*) pdata;
                        break;
                }
		pdata+=2;
        }
        return ret;
}

void* memfindref8(const void* data, unsigned int size, const void* search) {
	void *ref = memfind8(data, size, search);
	if (ref==NULL) return NULL;
	return memfind4(data, size, (unsigned int) ref);
}
