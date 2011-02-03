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
#include "payload.h"
#include "patching.h"

void prepare_vndevice() {
	struct vn_ioctl vn;

	int fd = open("/dev/vn0", O_RDONLY, 0);
	if (fd < 0) {
	    	printf("Can't open /dev/vn0 special file.\n");
		exit(1);
	}

	// detach
	ioctl(fd, VNIOCDETACH, &vn);

	// attach
	vn.vn_file = "/usr/lib/hfs_mdb";
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
	
	printf("Triggering kernel exploit!\n");

	int ret = mount("hfs", "/mnt/", MNT_RDONLY, (void*) &mount_args);
	
	printf("Payload returned successfully.\n");
}

int patch_kernel(unsigned char* address, unsigned int size) {
	unsigned int target = 0;
	/*
	AMFI: 00 B1 00 24 20 46 90 BD  +  0 => 00 B1 01 24 20 46 90 BD ; armv7
	      00 00 50 E3 00 00 00 0A  00 40 A0 E3 04 00 A0 E1 + 8 => 01 40 A0 E3 ; armv6

	PROD: 00 23 00 94 01 95 02 95  + 10 => 00 20 00 20 ; armv7 & armv6

	ECID: 02 90 03 90 1D 49 50 46  + 12 => 00 20 00 20 ; armv7
	      02 90 03 90 06 9A 07 9B  + 12 => 00 20 00 20 ; armv6

	SHSH: D3 80 04 98 02 21 7C 4B  +  8 => 00 20 ; 4.0 armv7
	      98 47 50 B9 00 98 02 21  +  8 => 00 20 ; 3.0 armv7
	      0D D1 01 98 02 21 34 4B  +  8 => 00 20 ; 4.0 armv6


	????: 00 28 40 F0 CC 80 04 98  +  8 => 00 20 00 20 ; 4.0 armv7
	      28 B9 00 98 FF F7 03 FF  +  8 => 00 20 00 20 ; 3.0 armv7
	      07 D1 01 98 FF F7 FC FE  +  8 => 00 20 00 20 ; 4.0 armv6


	????: 1F 4C 1E E0 28 46 51 46  +  8 => 01 20 01 20

	SHA1: A0 47 08 B1 28 46 30 E0  +  8 => 00 20 00 20
          85 68 00 23 .. 93 .. 93 -        .. .. .. .. 29 46 04 22
	TFP0: 85 68 00 23 02 93 01 93  +  8 => 0B E0 C0 46 ; armv7
	      85 68 .. 93 .. 93 00 2c          0B D1
	      85 68 02 93 01 93 00 2C  +  8 => 0E 93 BD 93 ; armv6
	*/
	unsigned int i = 0;
	unsigned char *paddress = address - 2;
	
	IOLog("Entering patch_kernel()\n");

	// temporary hardcoded AMFI patch
#ifdef FW_421
#ifdef IPOD2G
	_memcpy(OFFSET_AMFI_PATCH, "\x01\x40\xA0\xE3", 4);
#else
	_memcpy(OFFSET_AMFI_PATCH, "\x00\xB1\x01\x24\x20\x46\x90\xBD", 8);
#endif
#endif

	for(i = 0; i < size; i+=2) {
		paddress+=2;
		// Patch 2
		if(mem8eq(paddress, "\x00\xB1\x00\x24\x20\x46\x90\xBD")) {
			target = i + 0;
			IOLog("Found AMFI armv7 kernel patch at %p\n", &address[target]);
			_memcpy((char*) &address[target], "\x00\xB1\x01\x24\x20\x46\x90\xBD", 8);
			continue;
		}
		if(mem16eq(paddress, "\x00\x00\x50\xE3\x00\x00\x00\x0A\x00\x40\xA0\xE3\x04\x00\xA0\xE1")) {
			target = i + 8;
			IOLog("Found AMFI armv6 kernel patch at %p\n", &address[target]);
			_memcpy((char*) &address[target], "\x01\x40\xA0\xE3", 4);
			continue;
		}

		// Patch 6
		if(mem8eq(paddress, "\x00\x28\x40\xF0\xCC\x80\x04\x98")
				|| mem8eq(paddress, "\x28\xB9\x00\x98\xFF\xF7\x03\xFF")) {
			target = i + 8;
			IOLog("Found kernel patch 6 at %p\n", &address[target]);
			_memcpy(&address[target], "\x00\x20\x00\x20", 4);
			continue;
		}

		// Patch 7
		if(mem8eq(paddress, "\x1F\x4C\x1E\xE0\x28\x46\x51\x46")) {
			target = i + 8;
			IOLog("Found kernel patch 7 at %p\n", &address[target]);
			_memcpy(&address[target], "\x01\x20\x01\x20", 4);
			continue;
		}

		// Patch 8
		if(mem8eq(paddress, "\xA0\x47\x08\xB1\x28\x46\x30\xE0")) {
			target = i + 8;
			IOLog("Found kernel patch 8 at %p\n", &address[target]);
			_memcpy(&address[target], "\x00\x20\x00\x20", 4);
			continue;
		}
	}
	
	return 0;
}

static __attribute__ ((noinline)) void real_payload() {
	patch_kernel((void*) 0x80000000, 0xA00000);
	patch_sandbox((void*) 0x80000000, 0xA00000);
}

__attribute__ ((naked)) void payload() {
	real_payload();

	asm (
		// hfs_MountHFSVolume epilog:
		"mov	r0, #0x10\n\t"
		"lsl	r0, #0x8\n\t"	// put a readable page address in r0

		"add     sp, sp, #0xE4\n\t"
        	"pop     {r2-r4}\n\t"
        	"mov     r8, r2\n\t"
        	"mov     r10, r3\n\t"
		"mov     r11, r4\n\t"
		"pop     {r4-r7,pc}"
	);
}

int one = 1;
int zero = 0;
char* execve_env[]= {NULL};
char* execve_params[]={"/sbin/punchd", NULL};
char* animate_params[]={"/usr/bin/animate", NULL};

int main(int argc, char* argv[]) {
	// make sure this is disabled
	sysctlbyname("security.mac.vnode_enforce", NULL, 0, &zero, sizeof(one));
	sysctlbyname("vm.cs_validation", NULL, 0, &zero, sizeof(one));

	prepare_vndevice();
	mount_evil_hfs();

	// restore vnode_enforce and run launchd
	sysctlbyname("security.mac.vnode_enforce", NULL, 0, &one, sizeof(one));   
	if(vfork() != 0) {
		// Launch real launchd
		execve(execve_params[0], execve_params, execve_env);

	} else {
		// If /usr/bin/animate exists run it
		struct stat st;
		if(stat(animate_params[0], &st) == 0) {
			execve(animate_params[0], animate_params, execve_env);
		}
	}
	//execve(execve_params[0], execve_params, execve_env);
}

