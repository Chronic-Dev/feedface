PLATFORM=/Developer/Platforms/iPhoneOS.platform
SDKVER=4.2
SDK=$(PLATFORM)/Developer/SDKs/iPhoneOS$(SDKVER).sdk
CC=$(PLATFORM)/Developer/usr/bin/gcc-4.2
LD=$(PLATFORM)/Developer/usr/bin/ld
AS=$(PLATFORM)/Developer/usr/bin/as

CCFLAGS=-arch armv6 -isysroot=$(SDK) -I./include -I$(SDK)/usr/include -L$(SDK)/usr/lib -lc

all:
	$(CC) $(CCFLAGS) -o payload *.c
	#arm-elf-as -mthumb --fatal-warnings -o kpayload.o kpayload.S
	arm-elf-gcc kpayload.S -o kpayload.o -Ttext=0x010000 -nostdlib 
	arm-elf-objcopy -O binary  kpayload.o kpayload.bin
	rm kpayload.o
