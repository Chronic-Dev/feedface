/*
 * libmacho - macho.c
 * Functions for parsing and modifying Mach-O files
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "macho.h"
#include "common.h"

void macho_debug_header(macho_header_t* header) {
	debug("Mach-O Header\n");
	debug("\tmagic = 0x%08x\n", header->magic);
	debug("\tcputype = 0x%08x\n", header->cputype);
	debug("\tcpusubtype = 0x%08x\n", header->cpusubtype);
	debug("\tfiletype = 0x%08x\n", header->filetype);
	debug("\tncmds = 0x%08x\n", header->ncmds);
	debug("\tsizeofcmds = 0x%08x\n", header->sizeofcmds);
	debug("\tflags = 0x%08x\n\n", header->flags);
}

void macho_debug_symtab_cmd(macho_symtab_cmd_t* cmd) {
	debug("Mach-O Symbol Table Load Command\n");
	debug("\tcmd = 0x%08x\n", cmd->cmd);
	debug("\tcmdsize = 0x%08x\n", cmd->cmdsize);
	debug("\tsymoff = 0x%08x\n", cmd->symoff);
	debug("\tnsyms = 0x%08x\n", cmd->nsyms);
	debug("\tstroff = 0x%08x\n", cmd->stroff);
	debug("\tstrsize = 0x%08x\n\n", cmd->strsize);
}

void macho_debug_nlist(macho_nlist_t* nlist) {
	debug("Mach-O NList Entry\n");
	debug("\tun.strx = 0x%08x\n", nlist->un.strx);
	debug("\ttype = 0x%08x\n", nlist->type);
	debug("\tsect = 0x%08x\n", nlist->sect);
	debug("\tdesc = 0x%08x\n", nlist->desc);
	debug("\tvalue = 0x%08x\n", nlist->value);
}

void* macho_find_symbol(unsigned char* data, const char* symname) {
	int i = 0;
	int j = 0;
	unsigned int offset = 0;
	macho_header_t* header = (macho_header_t*) data;

	if(header->magic == MACHO_MAGIC_THIN) {
		//debug("Found thin mach-o file\n");
		offset += sizeof(macho_header_t);
		//macho_debug_header(header);

		for(i = 0; i < header->ncmds; i++) {
			macho_load_cmd_t* load = (macho_load_cmd_t*) &data[offset];
			if(load->cmd == MACHO_CMD_SYMTAB) {
				macho_symtab_cmd_t* symtab = (macho_symtab_cmd_t*) &data[offset];
				//macho_debug_symtab_cmd(symtab);
			
				unsigned int offset2 = symtab->symoff;

				for(j = 0; j < symtab->nsyms; j++) {
					macho_nlist_t* nlist = (macho_nlist_t*) &data[offset2];
					//macho_debug_nlist(nlist);

					debug("%08x %08x\n", symtab->stroff, nlist->un.strx);

					continue;
					char* name = (char*) &data[symtab->stroff + nlist->un.strx];
					if(!strcmp(name, symname)) {
						debug("Functions %s at 0x%x\n", name, nlist->value);
						return (void*) nlist->value;
					} else {
						debug("%s\n", name);
					} 
					offset2 += sizeof(macho_nlist_t);
				}
			}
			offset += load->cmdsize;
		}
	} else if(header->magic == MACHO_MAGIC_FAT) {
		// This Mach-O file runs on multiple architectures
		debug("Found fat mach-o file\n");
		debug("Not implemented yet\n");

	} else {
		// WTF is this??! This isn't a Mach-O file!!!
		error("Found unknown file (0x%08x)\n", header->magic);
		return NULL;
	}

	return NULL;
}
