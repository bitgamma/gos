#!/usr/env python

import sys, os, struct

img = sys.argv[1]
stats = os.stat(img)
blocks = (stats.st_size // 512)

spt = 63
hds = 16

tmp = blocks // spt
sect = (blocks % spt) + 1
head = tmp % hds
cyl = tmp // hds

sysid = 0x7F
partition = struct.pack('<BBBBBBBBII', 0x80, 0, 2, 0, sysid, head, ((cyl & 0x300) | sect), (cyl & 0xff), 1, (blocks - 1))

with open(img, "rb+") as f:
  f.seek(446)
  f.write(partition)
