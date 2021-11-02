#!/usr/env python

import sys, os, struct, math

img = sys.argv[1]
stage2 = sys.argv[2]
kernel = sys.argv[3]

stats = os.stat(img)
blocks = (stats.st_size // 512)

stage2_stats = os.stat(stage2)
stage2_blocks = math.ceil(stage2_stats.st_size / 512)

kernel_stats = os.stat(kernel)
kernel_blocks = math.ceil(kernel_stats.st_size / 512)

spt = 63
hds = 16

tmp = blocks // spt
sect = (blocks % spt) + 1
head = tmp % hds
cyl = tmp // hds

sysid = 0x7F
partition = struct.pack('<IIBBBBBBBBII', stage2_blocks, kernel_blocks, 0x80, 0, 2, 0, sysid, head, ((cyl & 0x300) | sect), (cyl & 0xff), 1, (blocks - 1))

with open(img, "rb+") as f:
  f.seek(438)
  f.write(partition)
