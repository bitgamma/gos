#!/usr/env python

import sys
from PIL import Image

infile = sys.argv[1]
outfile = sys.argv[2]
pal = sys.argv[3]

with Image.open(pal) as palette, open(outfile, "wb") as bin:
  with Image.open(infile) as img:
    img = img.convert('RGB')
    img = img.quantize(palette=palette)
    bin.write(img.tobytes())