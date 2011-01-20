/*
 * libmacho - macho.h
 * Header for parsing and modifying Mach-O files
 *
 * Copyright (c) 2010 Joshua Hill. All Rights Reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef LIBMACHO_H
#define LIBMACHO_H

#ifdef __cplusplus
extern "C" {
#endif

// Mach-O Magic Values
#define MACHO_MAGIC_THIN 0xfeedface
#define MACHO_MAGIC_FAT 0xcafebabe // ????

// Mach-O Load Commands
#define MACHO_CMD_SEGMENT 0x1
#define MACHO_CMD_SYMTAB 0x2
#define MACHO_CMD_SYMSEG 0x3
#define MACHO_CMD_THREAD 0x4
#define MACHO_CMD_UNIXTHREAD 0x5
#define MACHO_CMD_LOADFVMLIB 0x6
#define MACHO_CMD_IDFVMLIB 0x7
#define MACHO_CMD_IDENT 0x8
#define MACHO_CMD_FVMFILE 0x9
#define MACHO_CMD_PREPAGE 0xa
#define MACHO_CMD_DYSYMTAB 0xb
#define MACHO_CMD_LOAD_DYLIB 0xc
#define MACHO_CMD_ID_DYLIB 0xd
#define MACHO_CMD_LOAD_DYLINKER 0xe
#define MACHO_CMD_ID_DYLINKER 0xf
#define MACHO_CMD_PREBOUND_DYLIB 0x10
#define MACHO_CMD_ROUTINES 0x11
#define MACHO_CMD_SUB_FRAMEWORK 0x12
#define MACHO_CMD_SUB_UMBRELLA 0x13
#define MACHO_CMD_SUB_CLIENT 0x14
#define MACHO_CMD_SUB_LIBRARY 0x15
#define MACHO_CMD_TWOLEVEL_HINTS 0x16
#define MACHO_CMD_PREBIND_CKSUM 0x17
//#define MACHO_CMD_ 0x18
//#define MACHO_CMD_ 0x19
//#define MACHO_CMD_ 0x1a
#define MACHO_CMD_UUID 0x1b


/*
 * Mach-O File Structures
 */
typedef struct {
	unsigned int magic;
	unsigned int cputype;
	unsigned int cpusubtype;
	unsigned int filetype;
	unsigned int ncmds;
	unsigned int sizeofcmds;
	unsigned int flags;
} macho_header_t;

typedef struct {
	unsigned int cmd;
	unsigned int cmdsize;
} macho_load_cmd_t;

typedef struct {
    unsigned int cmd;
    unsigned int cmdsize;
    unsigned char segname[16];
    unsigned int vmaddr;
    unsigned int vmsize;
    unsigned int fileoff;
    unsigned int filesize;
    unsigned int maxprot;
    unsigned int initprot;
    unsigned int nsects;
    unsigned int flags;
} macho_segment_cmd_t;

typedef struct {
	unsigned int cmd;
	unsigned int cmdsize;
	unsigned int symoff;
	unsigned int nsyms;
	unsigned int stroff;
	unsigned int strsize;
} macho_symtab_cmd_t;

typedef struct {
	unsigned int cmd;
	unsigned int cmdsize;
	unsigned char uuid[16];
} macho_uuid_cmd_t;

typedef struct {
	unsigned int r[13];
	unsigned int sp;
	unsigned int lr;
	unsigned int pc;
	unsigned int cpsr;
} macho_arm_thread_state_t;

typedef struct {
	unsigned int cmd;
	unsigned int cmdsize;
	unsigned int flavor;
	unsigned int count;
	macho_arm_thread_state_t state;
} macho_unixthread_cmd_t;

typedef struct {
	unsigned char sectname[16];
	unsigned char segname[16];
	unsigned int addr;
	unsigned int size;
	unsigned int offset;
	unsigned int align;
	unsigned int reloff;
	unsigned int nreloc;
	unsigned int flags;
	unsigned int reserved1;
	unsigned int reserved2;
} macho_segment_sect_t;

typedef struct {
	union {
		int strx;
	} un;
	unsigned char type;
	unsigned char sect;
	signed short desc;
	unsigned int value;
} macho_nlist_t;

typedef struct {
	unsigned int offset;
	const char* name;
	void* address;
} macho_symbol_t;

void* macho_find_symbol(unsigned char* data, const char* symname);


#ifdef __cplusplus
}
#endif

#endif // LIBMACHO_H
