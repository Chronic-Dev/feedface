#ifndef COMMON_H
#define COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#define DEBUG 1

#define info(...) printf(__VA_ARGS__)
#define error(...) fprintf(stderr, __VA_ARGS__)
#define debug(...) if(_debug) fprintf(stderr, __VA_ARGS__)

extern int _debug;

int read_file(const char* filename, unsigned char** data, unsigned int* size);
int write_file(const char* filename, const unsigned char* data, unsigned int size);

#ifdef __cplusplus
}
#endif

#endif
