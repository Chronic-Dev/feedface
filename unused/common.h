#ifndef COMMON_H
#define COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#define DEBUG 1

extern int(*myprintf)(const char *fmt, ...);

#define info(...)  myprinf(__VA_ARGS__)
#define error(...) myprintf( __VA_ARGS__)
#define debug(...) if(_debug) myprintf( __VA_ARGS__)

extern int _debug;

int read_file(const char* filename, unsigned char** data, unsigned int* size);
int write_file(const char* filename, const unsigned char* data, unsigned int size);

#ifdef __cplusplus
}
#endif

#endif
