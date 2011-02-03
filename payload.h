#ifndef PAYLOAD_H
#define PAYLOAD_H

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>
#include <sys/mount.h>

#define FW_421
#define IPHONE3GS

#ifdef FW_421
#ifdef IPHONE3GS
	#define IOLog ((void (*)(char *fmt, ...)) 0x801a8a79)
	#define kprintf ((void (*)(char *fmt, ...)) 0x801dd879)
	#define kalloc ((void *(*)(unsigned int)) 0x80019021)
	#define flush_dcache ((void (*)(void *addr, unsigned size, int phys)) 0x8005e29d)
	#define invalidate_icache ((void (*)(void *addr, unsigned size, int phys)) 0x8005e7cc)
	#define sb_evaluate 0x804008B1
	#define _vn_getpath 0x80088771
	#define OFFSET_AMFI_PATCH 0x803E604C
#endif // IPHONE3GS

#ifdef IPHONE4
	#define IOLog ((void (*)(char *fmt, ...)) 0x801a8a79)
	#define kprintf ((void (*)(char *fmt, ...)) 0x801dd87d)
	#define kalloc ((void *(*)(unsigned int)) 0x80019021)
	#define flush_dcache ((void (*)(void *addr, unsigned size, int phys)) 0x8005e29d)
	#define invalidate_icache ((void (*)(void *addr, unsigned size, int phys)) 0x8005e7cc)
	#define sb_evaluate 0x804048B1
	#define _vn_getpath 0x80088771
	#define OFFSET_AMFI_PATCH 0x803EA04C
#endif // IPHONE4

#ifdef IPHONE42
	#define IOLog ((void (*)(char *fmt, ...)) 0x801a8b59)
	#define kprintf ((void (*)(char *fmt, ...)) 0x801dd95d)
	#define kalloc ((void *(*)(unsigned int)) 0x80019021)
	#define flush_dcache ((void (*)(void *addr, unsigned size, int phys)) 0x8005e2ad)
	#define invalidate_icache ((void (*)(void *addr, unsigned size, int phys)) 0x8005e80c)
	#define sb_evaluate 0x804008B1
	#define _vn_getpath 0x80088771
	#define OFFSET_AMFI_PATCH 0x803E6052
#endif // IPHONE42

#ifdef IPOD2G
	#define IOLog ((void (*)(char *fmt, ...)) 0x801a6be9)
	#define kprintf ((void (*)(char *fmt, ...)) 0x801dcccd)
	#define kalloc ((void *(*)(unsigned int)) 0x80019e2d)
	#define flush_dcache ((void (*)(void *addr, unsigned size, int phys)) 0x8005fd41)
	#define invalidate_icache ((void (*)(void *addr, unsigned size, int phys)) 0x8005ffb8)
	#define sb_evaluate 0x803d1b94
	#define _vn_getpath 0x80087f53
	#define OFFSET_AMFI_PATCH 0x0803B508C
#endif // IPOD2G

#ifdef IPOD3G
	#define IOLog ((void (*)(char *fmt, ...)) 0x801a8a79)
	#define kprintf ((void (*)(char *fmt, ...)) 0x801dd879)
	#define kalloc ((void *(*)(unsigned int)) 0x80019021)
	#define flush_dcache ((void (*)(void *addr, unsigned size, int phys)) 0x8005e29d)
	#define invalidate_icache ((void (*)(void *addr, unsigned size, int phys)) 0x8005e7cc)
	#define sb_evaluate 0x803FD8B1
	#define _vn_getpath 0x80088771
	#define OFFSET_AMFI_PATCH 0x803e304c
#endif //IPOD3G

#ifdef IPOD4G
	#define IOLog ((void (*)(char *fmt, ...)) 0x801a8a79)
	#define kprintf ((void (*)(char *fmt, ...)) 0x801dd87d)
	#define kalloc ((void *(*)(unsigned int)) 0x80019021)
	#define flush_dcache ((void (*)(void *addr, unsigned size, int phys)) 0x8005e29d)
	#define invalidate_icache ((void (*)(void *addr, unsigned size, int phys)) 0x8005e7cc)
	#define sb_evaluate 0x804028B1
	#define _vn_getpath 0x80088771
	#define OFFSET_AMFI_PATCH 0x803E804C
#endif // IPOD4G

#ifdef IPAD1G
	#define IOLog ((void (*)(char *fmt, ...)) 0x801a8a79)
	#define kprintf ((void (*)(char *fmt, ...)) 0x801dd87d)
	#define kalloc ((void *(*)(unsigned int)) 0x80019021)
	#define flush_dcache ((void (*)(void *addr, unsigned size, int phys)) 0x8005e29d)
	#define invalidate_icache ((void (*)(void *addr, unsigned size, int phys)) 0x8005e7cc)
	#define sb_evaluate 0x804028B1
	#define _vn_getpath 0x80088771
	#define OFFSET_AMFI_PATCH 0x803E804C
#endif // IPAD1G

#ifdef APPLETV2G
	#define IOLog ((void (*)(char *fmt, ...)) 0x801a8a79)
	#define kprintf ((void (*)(char *fmt, ...)) 0x801dd87d)
	#define kalloc ((void *(*)(unsigned int)) 0x80019021)
	#define flush_dcache ((void (*)(void *addr, unsigned size, int phys)) 0x8005e29d)
	#define invalidate_icache ((void (*)(void *addr, unsigned size, int phys)) 0x8005e7cc)
	#define sb_evaluate 0x803308B1
	#define _vn_getpath 0x80088771
	#define OFFSET_AMFI_PATCH 0x8031604C
#endif // APPLETV2G

#endif // FW_421

int (*vn_getpath) (void *vp, char *pathbuf, int *len);

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
