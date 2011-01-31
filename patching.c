#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "patching.h"
#include "payload.h"


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

void dump(void *addr, unsigned int size) {
	unsigned int i;
	unsigned int count = size >> 2;
	unsigned int *daddr = (unsigned int *) addr;

	for (i = 0; i < count; i+=4) {
		IOLog("%08x %08x %08x %08x\n", daddr[i], daddr[i+1], daddr[i+2], daddr[i+3]);
	}
}
