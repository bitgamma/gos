#!/usr/env python
"""
 Copyright (c) 2021 Michele Balistreri

 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at http://mozilla.org/MPL/2.0/.
"""

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