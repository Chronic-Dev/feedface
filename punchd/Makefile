PLATFORM=/Developer/Platforms/iPhoneOS.platform
SDKVER=4.2
SDK=$(PLATFORM)/Developer/SDKs/iPhoneOS$(SDKVER).sdk
CC=$(PLATFORM)/Developer/usr/bin/gcc-4.2
LD=$(PLATFORM)/Developer/usr/bin/ld
AS=$(PLATFORM)/Developer/usr/bin/as

CCFLAGS=-arch armv6 -isysroot=$(SDK) -I./include -I$(SDK)/usr/include -L$(SDK)/usr/lib
CCFLAGS_DYLIB=$(CCFLAGS) -segaddr __ROP 0x11140000 -segaddr __TEXT 0x11120000 -segaddr __LOL 0x1000 -Wl,-stack_addr,0x11190000 -Wl,-stack_size,0x10000 -strip -segprot __TEXT rw rw -segaddr __DATA 0x11190000 
#CCFLAGS_DYLIB=$(CCFLAGS) -segaddr __ROP 0x11140000 -segaddr __TEXT 0x11120000 -shared

all: punchd_placeholders

punchd_placeholders: punchd.c
	$(CC) $(CCFLAGS_DYLIB) -o $@ $^
	python fix_offset78.py punchd_placeholders
	cd config && python three.py 
	
clean:
	rm punchd_placeholders punchd

