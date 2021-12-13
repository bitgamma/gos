#!/usr/env python
"""
 Copyright (c) 2021 Michele Balistreri

 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at http://mozilla.org/MPL/2.0/.
"""

import os, sys, shutil, wave, yaml
from PIL import Image

T_NONE = 0
T_IMG = 1
T_OPL = 2
T_WAV = 3

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
    if palette is not None:
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
    c.write(f"fmt_pcm_context_t {ident} = {{0x464d4350, {rate}, {frames}, {channels}, (void*){origin}, 0, 0}};\n")

def listres(dir, files, cident):
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

def genbootcfg(config, preload, bootcfg):
  with open(bootcfg, "w") as f:
    if preload:
      f.write("payload_size equ system_size\n")
    else:
      f.write("payload_size equ kernel_size\n")

    for k, v in config.items():
      f.write(f"{k} equ {v}\n")

def genkernelcfg(config, kernelcfg):
  with open(kernelcfg, "w") as f:
    f.write("#ifndef __CONFIG__\n#define __CONFIG__\n\n")
    for k, v in config.items():
      f.write(f"#define {k} {v}\n")
    f.write("\n#endif")

config = None
with open(sys.argv[1], "r") as cfgfile:
  config = yaml.safe_load(cfgfile)

appdir = os.path.dirname(sys.argv[1])

cfile = os.path.join(appdir, "src", "res.c")
hfile = os.path.join(appdir, "inc", "res.h")
resdir = os.path.join(appdir, "res")
binfile = os.path.join(appdir, "..", "build", "bin", "res.bin")
bootcfg = os.path.join(appdir, "..", "bootloader", "config.inc")
kernelcfg = os.path.join(appdir, "..", "os", "inc", "config.h")

resconfig = config.get("resources", {})
preload = resconfig.get("preload_resources", True)
subdirs = resconfig.get("subdirs", [])

genbootcfg(config.get("bootloader", {}), preload, bootcfg)
genkernelcfg(config.get("kernel", {}), kernelcfg)


palette = None
if "palette" in resconfig:
  palette = Image.open(os.path.join(appdir, resconfig["palette"]))

files = []
cident = []

listres(resdir, files, cident)

for subdir in subdirs:
  listres(os.path.join(resdir, subdir), files, cident)

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

  if preload:
    h.write("\nvoid res_init();\n\n")
    geninit(c, cident)

  h.write("#endif")
