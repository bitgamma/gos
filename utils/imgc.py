#!/usr/env python

import os, sys
from PIL import Image

def geninit(c, idents):
  c.write("\nvoid res_init() {\n")
  c.write("  uint32_t base = (SYSTEM_ADDR + (512 * KERNEL_SIZE));\n\n")
  for ident in idents:
    c.write(f"  {ident}.data += base;\n")
  c.write("}\n")

def genc(c, h, ident, origin, width, height):
  h.write(f"extern td_image_t {ident};\n")
  c.write(f"td_image_t {ident} = {{{width}, {height}, (void*)({origin})}};\n")

def processfile(c, h, bin, file, ident, palette):
  with Image.open(file) as img:
    img = img.convert('RGB')
    img = img.quantize(palette=palette)
    origin = bin.tell()
    align = (4 - (origin % 4)) % 4
    bin.seek(align, 1)
    origin += align
    bin.write(img.tobytes())
    genc(c, h, ident, origin, img.width, img.height)

cfile = sys.argv[1]
hfile = sys.argv[2]
dir = sys.argv[3]
binfile = sys.argv[4]
pal = sys.argv[5]

files = []
cident = []
palette = Image.open(pal)

for filename in os.listdir(dir):
  if filename.endswith(".png"):
    cident.append(f"res_{os.path.splitext(filename)[0]}")
    files.append(os.path.join(dir, filename))

with open(cfile, "w") as c, open(hfile, "w") as h, open(binfile, "wb") as bin:
  h.write("#ifndef __RES__\n#define __RES__\n\n#include <2d.h>\n\n")
  c.write("#include <mem.h>\n#include <2d.h>\n\n")

  for i, file in enumerate(files):
    processfile(c, h, bin, file, cident[i], palette)
  h.write("\nvoid res_init();\n\n")
  h.write("#endif")

  geninit(c, cident)
