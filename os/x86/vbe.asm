bits 16

%include "const.inc"

section .text

get_mode_info:
  mov ax, 0x4f01
  mov cx, 0x103
  mov di, vbe_mode_info
  int 0x10

set_vbe_mode:
  mov ax, 0x4f02
  mov bx, 0x4103
  int 0x10
  jmp $