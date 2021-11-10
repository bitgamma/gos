#!/usr/env python

import os, sys, shutil
from PIL import Image

def geninit(c, idents):
  c.write("\nvoid res_init() {\n")
  c.write("  uint32_t base = (SYSTEM_ADDR + (512 * KERNEL_SIZE));\n\n")
  for ident in idents:
    c.write(f"  {ident}.data += base;\n")
  c.write("}\n")

def placeorigin(bin):
  origin = bin.tell()
  align = (4 - (origin % 4)) % 4
  bin.seek(align, 1)
  origin += align
  return origin

def processimg(c, h, bin, file, ident, palette):
  with Image.open(file) as img:
    img = img.convert('RGB')
    img = img.quantize(palette=palette)
    origin = placeorigin(bin)
    bin.write(img.tobytes())
    h.write(f"extern td_image_t {ident};\n")
    c.write(f"td_image_t {ident} = {{{img.width}, {img.height}, (void*){origin}}};\n")

def processmus(c, h, bin, file, ident):
  with open(file, "rb") as music:
    origin = placeorigin(bin)
    shutil.copyfileobj(music, bin)
    len = bin.tell() - origin
    h.write(f"extern snd_source_t {ident};\n")
    c.write(f"snd_source_t {ident} = {{FMT_DRO, {len}, (void*){origin}}};\n")

T_NONE = 0
T_IMG = 1
T_MUS = 2

cfile = sys.argv[1]
hfile = sys.argv[2]
dir = sys.argv[3]
binfile = sys.argv[4]
pal = sys.argv[5]

files = []
cident = []
palette = Image.open(pal)

for filename in sorted(os.listdir(dir)):
  typ = T_NONE

  if filename.endswith(".png"):
    typ = T_IMG
  elif filename.endswith(".dro"):
    typ = T_MUS

  if typ != T_NONE:
    cident.append(f"res_{os.path.splitext(filename)[0]}")
    files.append((typ, os.path.join(dir, filename)))

with open(cfile, "w") as c, open(hfile, "w") as h, open(binfile, "wb") as bin:
  h.write("#ifndef __RES__\n#define __RES__\n\n#include <2d.h>\n#include <snd.h>\n\n")
  c.write("#include <res.h>\n#include <mem.h>\n\n")

  for i, (typ, file) in enumerate(files):
    if typ == T_IMG:
      processimg(c, h, bin, file, cident[i], palette)
    elif typ == T_MUS:
      processmus(c, h, bin, file, cident[i])

  h.write("\nvoid res_init();\n\n")
  h.write("#endif")

  geninit(c, cident)
