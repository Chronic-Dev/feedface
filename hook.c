#include "payload.h"
#include "patching.h"
//#include <sys/mman.h>

static void hook2() {
	//IOLog("hooked %08x %08x %08x\n");
}

__attribute ((naked, flatten)) void hook() {
	// hook: save all registers
	asm (
	"push {r0-r7, lr}\n\t"
	"mov r4, r8\n\t"
	"mov r5, r9\n\t"
	"mov r6, r10\n\t"
	"mov r7, r11\n\t"
	"push {r4-r7}\n\t"
	"sub sp, #4\n\t"
	);

	hook2();

	// hook: restore all registers
	asm (
	"add sp, #4\n\t"
	"pop {r4-r7}\n\t"
	"mov r8, r4\n\t"
	"mov r9, r5\n\t"
	"mov r10, r6\n\t"
	"mov r11, r7\n\t"
	"pop {r0-r7}\n\t"
	);

	// restores original lr, prepare r12 for jump, restore original r4 & r5
	asm (
	"pop {r4}\n\t"
	"mov lr, r4\n\t"
	"add r5, #7\n\t"
	"mov r12, r5\n\t"
	"pop {r4-r5}\n\t"
	);

	// original hooked function prolog
	asm (
	"push {r0-r3}\n\t"
	"push {r4-r7,lr}\n\t"
	"add r7, sp, #0xc\n\t"
	"sub sp, #4\n\t"
	"mov r3, #0\n\t"
	);

	// return to original function after overwritten prolog
	asm (
	"bx r12\n\t"
	"nop\n\t"
	".word 0xfeedface\n\t"
	);
}

static unsigned int hook_armbx(void *addr, void* to) {
	((unsigned int   *) addr)[0] = 0xe92d0030;         // PUSH {R4-R5}
	((unsigned int   *) addr)[1] = 0xe59f4004;         // LDR R4, =addr
	((unsigned int   *) addr)[2] = 0xe1a0500f;         // MOV R5, PC
	((unsigned int   *) addr)[3] = 0xe12fff14;         // BX R4
	((unsigned int   *) addr)[4] = (unsigned int) to;
	return 5 * 4;
}

static unsigned int hook_thumbbx(void *addr, void* to) {
	((unsigned short *) addr)[0] = 0xb430;         // PUSH {R4-R5}
	((unsigned short *) addr)[1] = 0x4c01;         // LDR R4, =addr
	((unsigned short *) addr)[2] = 0x467d;         // MOV R5, PC
	((unsigned short *) addr)[3] = 0x4720;         // BX R4
	((unsigned int   *) addr)[2] = (unsigned int) to;
	return 3 * 4;
}

void hook_thumb(void *addr, void* to) {
        unsigned int addrhook = (unsigned int) to;
        if (addrhook % 2 !=0) addrhook = addrhook - 1;
	unsigned int hsize = (unsigned int) memfind4((void*) addrhook, 0x1000, 0xfeedface) - addrhook;
	hsize += 0x100; // there's pool data after the payload
	unsigned char* kbuf = (unsigned char*) kalloc(hsize);
        _memcpy(kbuf, (void*) addrhook, hsize);
	hook_thumbbx(addr, (void *) (kbuf + 1));
	flush_dcache(kbuf, hsize, 0);
	invalidate_icache(kbuf, hsize, 0);
}

int patch_sandbox(void* address, unsigned int size) {
	//hook_arm((void *) 0x803D1B94, hook);
	hook_thumb((void *) 0x801DCCCC, hook);
	IOLog("hey ho\n");
	kprintf("heyho\n");
}

