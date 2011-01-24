#include "payload.h"
#include "patching.h"


__attribute__ ((flatten)) static unsigned int hook(unsigned int unk1, unsigned int unk2, unsigned int unk3, unsigned int unk4) {
	if (FALSE) {
		*((unsigned int *) unk1) = 0;
		((unsigned char *) unk1)[4] = 8;
		return unk1;
	} else {
		asm (
		// sb_evaluate overwritten epilog
		"push {r4-r7, lr}\n\t"
		"add r7, sp, #0xc\n\t"
		"mov r4, r8\n\t"
		"mov r5, r10\n\t"
		"mov r6, r11\n\t"
		"push {r4-r6}\n\t"
		// call sb_evaluate + 0xc
		"mov r0, %[unk1]\n\t"
		"mov r1, %[unk2]\n\t"
		"mov r2, %[unk3]\n\t"
		"mov r3, %[unk4]\n\t"
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

static void hook_armbx(void *addr, void* to) {
	((unsigned int   *) addr)[0] = 0xe59fc000;         // LDR R12, =addr
	((unsigned int   *) addr)[1] = 0xe12fff1c;         // BX R12
	((unsigned int   *) addr)[2] = (unsigned int) to;
}

static void hook_arm(void *addr, void* to) {
        unsigned int addrhook = (unsigned int) to;
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
		IOLog("Can't find the hooked sub address placeholder.\n");
		return;
	}

	*beefface = ((unsigned int) addr) + 0xc;
	flush_dcache(kbuf, hsize, 0);
	invalidate_icache(kbuf, hsize, 0);
	hook_armbx(addr, (void *) (kbuf + 1));
}

int patch_sandbox(void* address, unsigned int size) {
	hook_arm((void *) sb_evaluate, hook);
}

