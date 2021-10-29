bits 16

%include "const.inc"

section .text

global vbe_get_mode_info, vbe_set_mode

vbe_get_mode_info:
  push ebp
  mov ebp, esp
  mov ecx, [ebp+8]
  mov ax, 0x4f01
  mov di, vbe_mode_info
  int 0x10
  mov esp, ebp
  pop ebp
  ret

vbe_set_mode:
  push ebp
  mov ebp, esp
  mov ebx, [ebp+8]
  or bx, 0x4000
  mov ax, 0x4f02
  int 0x10
  mov esp, ebp
  pop ebp
  ret