; Copyright (c) 2021 Michele Balistreri
;
; This Source Code Form is subject to the terms of the Mozilla Public
; License, v. 2.0. If a copy of the MPL was not distributed with this
; file, You can obtain one at http://mozilla.org/MPL/2.0/.

bits 32

stack_top equ 0x10000
bss equ 0x10000
bss_size equ 0x20000

global _kstart
extern kernel_main

section .text.startup

_kstart:
  mov esp, stack_top
  xor ebp, ebp
zero_bss:
  cld
  mov eax, 0x00
  mov ecx, (bss_size >> 2)
  mov edi, bss
  rep stosd
  jmp kernel_main
