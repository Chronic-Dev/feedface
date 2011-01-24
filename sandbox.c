#include "payload.h"
#include "patching.h"

__attribute__ ((flatten)) static unsigned int sb_evaluate_hook_arm(unsigned int unk1, unsigned int unk2, unsigned int unk3, unsigned int unk4) {
	if (TRUE) {
		*((unsigned int *) unk1) = 0;
		((unsigned char *) unk1)[4] = 8;
		return unk1;
	} else {
		asm (
		// hooked function overwritten epilog
		"push {r4-r7, lr}\n\t"
		"add r7, sp, #0xc\n\t"
		"mov r4, r8\n\t"
		"mov r5, r10\n\t"
		"mov r6, r11\n\t"
		"push {r4-r6}\n\t"
		// original parameters
		"mov r0, %[unk1]\n\t"
		"mov r1, %[unk2]\n\t"
		"mov r2, %[unk3]\n\t"
		"mov r3, %[unk4]\n\t"
		// adjust the LR and call hooked function
		"mov r12, %[addr]\n\t"
		"mov r5, sp\n\t"
		"add r5, #0x1c\n\t"
		"mov r6, pc\n\t"
		"add r6, #0x5\n\t"
		"str r6, [r5]\n\t"
		"bx r12\n\t"
		"mov %[ret], r0\n\t"
		: [ret]"=r" (unk1)
		: [unk1]"r" (unk1), [unk2]"r" (unk2), [unk3]"r" (unk3), [unk4]"r" (unk4), [addr]"r" (0xbeefface)
		: "r4", "r5", "r6", "r7"
		);
	
		return unk1;
	}
}

__attribute__ ((flatten)) static unsigned int sb_evaluate_hook_thumb(unsigned int unk1, unsigned int unk2, unsigned int unk3, unsigned int unk4) {
	// we'll do this one after arm version's finished.
	*((unsigned int *) unk1) = 0;
	((unsigned char *) unk1)[4] = 8;
	return unk1;
}

__attribute__ ((flatten)) static unsigned int kprintf_hook_thumb(unsigned int unk1, unsigned int unk2, unsigned int unk3, unsigned int unk4) {
	IOLog("kprintf hooked !\n");
}

static void hook_armbx(void *addr, void* to) {
	((unsigned int   *) addr)[0] = 0xe59fc000;		// LDR R12, =addr
	((unsigned int   *) addr)[1] = 0xe12fff1c;		// BX R12
	((unsigned int   *) addr)[2] = (unsigned int) to;
}

static void hook_thumbbx(void *addr, void* to) {
	((unsigned short *) addr)[0] = 0xb480;			// PUSH {R7}
	((unsigned short *) addr)[1] = 0x4f02;			// LDR R7, =addr
	((unsigned short *) addr)[2] = 0x46bc;			// MOV R12, R7
	((unsigned short *) addr)[3] = 0xbc80;			// POP {R7}
	((unsigned short *) addr)[4] = 0x4760;			// Bx R12
	((unsigned short *) addr)[5] = 0x0000;
	((unsigned int   *) addr)[3] = (unsigned int) to;
}

static void hook_arm_thumb(void *addr, void* to, void* to_thumb) {
        unsigned int addrhook;
        if ((unsigned int) addr % 2 !=0) {
        	addrhook = (unsigned int) to_thumb;
        } else {
        	addrhook = (unsigned int) to;
	}

	if (addrhook % 2 !=0) addrhook = addrhook - 1;
	unsigned int hsize = 0x1000;

	unsigned char* kbuf = (unsigned char*) kalloc(hsize);
        if (kbuf==NULL) {
		IOLog("Can't allocate some kernel memory for the hook.\n");
		return;
	}
	_memcpy(kbuf, (void*) addrhook, hsize);
	
	volatile unsigned int *beefface = (unsigned int*) memfind4((void*) kbuf, hsize, 0xbeefface);
	if (beefface==NULL) {
		IOLog("Can't find the hooked sub address placeholder. Can't tell if it's attended or not.\n");
	} else {
	        if ((unsigned int) addr % 2 !=0) {
			*beefface = ((unsigned int) addr) + 0x10;
		} else {
			*beefface = ((unsigned int) addr) + 0xc;
		}
	}
	flush_dcache(kbuf, hsize, 0);
        if ((unsigned int) addr % 2 !=0) {
		hook_thumbbx(addr -1, (void *) (kbuf + 1));
	} else {
		hook_armbx(addr, (void *) (kbuf + 1));
	}
	invalidate_icache(kbuf, hsize, 0);
}

int patch_sandbox(void* address, unsigned int size) {
	hook_arm_thumb((void *) sb_evaluate, sb_evaluate_hook_arm, sb_evaluate_hook_thumb);
	//hook_arm_thumb((void *) kprintf, NULL, kprintf_hook_thumb);
	//kprintf("hook test\n");
}

