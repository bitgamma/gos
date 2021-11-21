#!/usr/env python
"""
 Copyright (c) 2021 Michele Balistreri

 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at http://mozilla.org/MPL/2.0/.
"""

import os, sys, shutil, wave
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

def processopl(c, h, bin, file, ident):
  with open(file, "rb") as music:
    origin = placeorigin(bin)
    shutil.copyfileobj(music, bin)
    h.write(f"extern fmt_dro_context_t {ident};\n")
    c.write(f"fmt_dro_context_t {ident} = {{(void*){origin}, NULL, NULL, 0, 0, 0, NULL}};\n")

def processwav(c, h, bin, file, ident):
  with wave.open(file, "rb") as music:
    if music.getsampwidth() != 2:
      sys.exit(-1)
    origin = placeorigin(bin)
    rate = music.getframerate()
    frames = music.getnframes()
    channels = music.getnchannels()
    bin.write(music.readframes(frames))
    frames = frames * channels * 2
    h.write(f"extern fmt_pcm_context_t {ident};\n")
    c.write(f"fmt_pcm_context_t {ident} = {{0x464d4350, {rate}, {frames}, {channels}, (void*){origin}, 0}};\n")

T_NONE = 0
T_IMG = 1
T_OPL = 2
T_WAV = 3

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
    typ = T_OPL
  elif filename.endswith(".wav"):
    typ = T_WAV

  if typ != T_NONE:
    cident.append(f"res_{os.path.splitext(filename)[0]}")
    files.append((typ, os.path.join(dir, filename)))

with open(cfile, "w") as c, open(hfile, "w") as h, open(binfile, "wb") as bin:
  h.write("#ifndef __RES__\n#define __RES__\n\n#include <2d.h>\n#include <fmt_dro.h>\n#include <fmt_pcm.h>\n\n")
  c.write("#include <res.h>\n#include <mem.h>\n\n")

  for i, (typ, file) in enumerate(files):
    if typ == T_IMG:
      processimg(c, h, bin, file, cident[i], palette)
    elif typ == T_OPL:
      processopl(c, h, bin, file, cident[i])
    elif typ == T_WAV:
      processwav(c, h, bin, file, cident[i])

  h.write("\nvoid res_init();\n\n")
  h.write("#endif")

  geninit(c, cident)
