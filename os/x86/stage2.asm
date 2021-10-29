bits 16

%include "const.inc"

section .text
extern enable_a20
extern vbe_select_mode
global _stage2, _die

_stage2:
  call enable_a20
  call vbe_select_mode
  cli
  jmp $

_die:
  jmp 0xffff:0