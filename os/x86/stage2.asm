bits 16

%include "const.inc"

section .text
extern enable_a20
extern vbe_select_mode, load_system
global _stage2, _die

_stage2:
  call enable_a20
  call vbe_select_mode
  call load_system
protected:
  cli
  mov ecx, kernel
  push ecx

  mov eax, cr0
  or al, 1
  mov cr0, eax

  jmp $+2

  mov ax, 0x10
  mov ds,ax
  mov es,ax
  mov fs,ax
  mov gs,ax
  mov ss,ax

  ret

_die:
  jmp 0xffff:0
