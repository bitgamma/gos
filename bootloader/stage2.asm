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

_die:
  jmp 0xffff:0

%include "a20.asm"
%include "bios.asm"

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