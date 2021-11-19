; Copyright (c) 2021 Michele Balistreri
;
; This Source Code Form is subject to the terms of the Mozilla Public
; License, v. 2.0. If a copy of the MPL was not distributed with this
; file, You can obtain one at http://mozilla.org/MPL/2.0/.

bits 16

%include "const.inc"

org stage2

section stage2

_stage2:
  mov ax, 0x0013
  int 0x10 ; shortcut to set chain 4 bit

  stc
  mov byte [pci_flags], 0
  mov ax, 0xb101
  int 0x1a
  jc pci_done
  mov [pci_flags], al
pci_done:
  clc

  call enable_a20
  call vbe_select_mode
  call show_bootlogo
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
  jmp kernel

bits 16
%include "a20.asm"
%include "bios.asm"

show_bootlogo:
  mov esi, bootlogo
  mov edi, [vbe_mode_fb]
  movzx ebx, word [vbe_mode_pitch]
  mov eax, ebx
  sub ebx, logo_width
  imul eax, logo_y
  add eax, logo_x
  add edi, eax
  mov eax, logo_height
draw_line:
  mov ecx, (logo_width >> 2)
  rep a32 movsd
  add edi, ebx
  dec eax
  jnz draw_line
  ret

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

align 4
bootlogo: incbin "bootlogo.bin"