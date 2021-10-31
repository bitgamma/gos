bits 32

%include "const.inc"

global _kstart
extern kernel_main

section .text.startup

_kstart:
zero_bss:
  cld
  mov eax, 0x00
  mov ecx, (bss_size >> 2)
  mov edi, bss
  rep stosd
  jmp kernel_main
