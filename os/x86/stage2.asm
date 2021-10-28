bits 16

%include "const.inc"

section .text
extern enable_a20
extern vbe_select_mode
global _stage2

_stage2:
  call enable_a20
  call vbe_select_mode
  jmp $