#!/usr/bin/python3
import sys,os,struct
#./genboot.py mbr.bin
#python genboot.py mbr.bin   argv[0] ---> argv[1]
try:
    f = open(sys.argv[1],'ab+')
except IOError:
    print('no file %s' %sys.argv[1])
    sys.exit(1)
print('%s'%sys.argv[1])
size = os.path.getsize(sys.argv[1]) # 文件路径及文件名
if(size > 510):
    print('ERROR: boot block too large: %d bytes (max 510)\n' %size)
    sys.exit(1)

print('OK: boot block is %d bytes (max 510)\n'%size)

s = '\0' * (510-size)
by = s.encode()
f.write(by)
f.write(b'\x55')
f.write(b'\xAA')
f.close()

