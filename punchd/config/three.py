#!/opt/local/bin/python2.6
import struct, sys, os
import warnings
warnings.simplefilter('error')

import config
cfg = config.openconfig()
arch = cfg['arch']
assert arch in ['armv6', 'armv7', 'i386']

cache = cfg.get('#dyld_shared_cache_armv7')
if not cache:
    cache = cfg.get('#dyld_shared_cache_armv6')

f=open("../punchd_placeholders2", "rb")
fi=f.read()
f.close()

import commands, struct,ctypes
BASE = 0

def pack_adjust_off(off):
    return struct.pack("<L", ctypes.c_uint32(off - BASE).value)

fi = fi.replace(struct.pack("<L", 0xFEED0007),pack_adjust_off(cache['k7']))
fi = fi.replace(struct.pack("<L", 0xFEED0011),pack_adjust_off(cache['k11']))
fi = fi.replace(struct.pack("<L", 0xFEED0012),pack_adjust_off(cache['k12']))

fi = fi.replace(struct.pack("<L", 0xDEAD0001),pack_adjust_off(cache['k101']))
fi = fi.replace(struct.pack("<L", 0xDEAD0002),pack_adjust_off(cache['k102']))
fi = fi.replace(struct.pack("<L", 0xDEAD0003),pack_adjust_off(cache['k103']))

f=open("../punchd","wb")
f.write(fi)
f.close()
