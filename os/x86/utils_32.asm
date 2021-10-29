bits 32

%include "const.inc"

section .text
global _jmp_to_kernel

_jmp_to_kernel:
  jmp 8:kernel