#ifndef PAYLOAD_H
#define PAYLOAD_H

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>
#include <sys/mount.h>

/*
 * CHANGE THESE!!!!!
 */
#define IOLog ((void (*)(char *fmt, ...)) 0x801A6BE9)
#define kprintf ((void (*)(char *fmt, ...)) 0x801DCCCD)
#define kalloc ((void *(*)(unsigned int)) 0x80019E2D)
#define flush_dcache ((void (*)(void *addr, unsigned size, int phys)) 0x8005FD41)
#define invalidate_icache ((void (*)(void *addr, unsigned size, int phys)) 0x8005FFB8)
#define sb_evaluate 0x803D1B94

struct hfs_mount_args {
    char     *fspec;
    uid_t     hfs_uid;
    gid_t     hfs_gid;
    mode_t    hfs_mask;
    u_int32_t hfs_encoding;
    struct    timezone hfs_timezone;
    int       flags;
    int       journal_tbuffer_size;
    int       journal_flags;
    int       journal_disable;
};

#ifdef __cplusplus
}
#endif

#endif
