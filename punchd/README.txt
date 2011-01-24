Incomplete codesign - variation of comex's launchd mod_initializers trick
it all happens in dyld's ImageLoaderMachO::doModInitFunctions() (not the open source one)

1. find gadgets addrs for target device
    cd config
    python config.py iPhone3,1_4.2.1
2. compile executable and patch gadgets palceholders
    make
3. boot ramdisk, mount data & system partitions
    /mnt1/sbin/launchd => /mnt1/sbin/punchd 
    punchd =>  /mnt1/sbin/launchd
    chmod +x /mnt1/sbin/punchd
    payload  =>   /mnt1/usr/lib/kern_sploit
    chmod +x /mnt1/usr/lib/kern_sploit
    
4. reboot and kern_sploit should be running at pid=1