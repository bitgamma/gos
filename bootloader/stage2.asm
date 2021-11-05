bits 16

%include "const.inc"

org stage2

section stage2

_stage2:
  call enable_a20
  call vbe_select_mode
  call load_system
protected:
  cli
  mov eax, cr0
  or al, 1
  mov cr0, eax
  jmp 8:pmode

bits 32
pmode:
  mov eax, 0x10
  mov ds,ax
  mov es,ax
  mov fs,ax
  mov gs,ax
  mov ss,ax
  mov esp, stack_top
  xor ebp, ebp
  jmp kernel

bits 16
%include "a20.asm"
%include "bios.asm"

_die:
  mov ax, 0x0007
  int 0x10
print:
  lodsb
  or al, al
  jz print_end
  mov ah, 0x0e
  mov bx, 9
  int 0x10
  jmp print
print_end:
  jmp $