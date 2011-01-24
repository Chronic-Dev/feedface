import struct,sys, os

"""
ugly script, when need to have a gadget ptr at offset 0x78 of the binary
we put the LC_UNIXTHREAD over there
"""
data = open(sys.argv[1], "rb").read()

UNIXTHREAD_HEADER = "05000000540000000100000011000000".decode("hex")
unixthread_off = data.find(UNIXTHREAD_HEADER)
unixthread = data[unixthread_off+16:unixthread_off+16+0x11*4]
regs = list(struct.unpack('I'*0x11, unixthread))

regs[5] = 0xDEAD0002
data2 = data.replace(UNIXTHREAD_HEADER + unixthread, "")

unixthread = UNIXTHREAD_HEADER + struct.pack('I'*0x11, *tuple(regs))

data2 = data2[:0x54] + unixthread + data2[0x54:]

binname = "%s2" % sys.argv[1]
open(binname,"wb").write(data2) 
os.chmod(binname, 755)
