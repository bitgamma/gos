bits 16

%include "const.inc"

section .text
extern enable_a20
extern vbe_select_mode, load_system
extern _jmp_to_kernel
global _stage2

_stage2:
  call enable_a20
  call vbe_select_mode
  or ax,ax
  jz die

  call load_system
  or ax,ax
  jz die

protected:
  cli
  mov eax, cr0
  or al, 1
  mov cr0, eax

  jmp $+2

  mov bx, 0x10
  mov ds, bx
  mov es, bx

  jmp _jmp_to_kernel

die:
  jmp _die

