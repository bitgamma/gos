bits 16

%include "const.inc"

section .text
extern enable_a20
extern vbe_select_mode, load_system
extern _jmp_to_kernel
global _stage2, _die

_stage2:
  call enable_a20
  call vbe_select_mode
  call load_system

  mov eax, disk_rb
  mov ecx, [eax]
  cmp ecx, 0x158bc031
  jne _die
  jmp $
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

_die:
  jmp 0xffff:0

