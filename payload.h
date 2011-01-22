#ifndef PAYLOAD_H
#define PAYLOAD_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * CHANGE THESE!!!!!
 */
#define kprintf ((void (*)(char *fmt, ...)) 0x801A6BE9)
#define kalloc ((void *(*)(unsigned int)) 0x80019E2D)

//#define copyin ((void (*)(void *uaddr, void *kaddr, size_t len)) 0x80066c61)
//#define lck_rw_lock_exclusive ((void (*)(void *)) 0x80061bc1)
//#define lck_rw_done ((void (*)(void *)) 0x80061ec1)

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
