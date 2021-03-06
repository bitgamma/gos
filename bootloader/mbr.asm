; Copyright (c) 2021 Michele Balistreri
;
; This Source Code Form is subject to the terms of the Mozilla Public
; License, v. 2.0. If a copy of the MPL was not distributed with this
; file, You can obtain one at http://mozilla.org/MPL/2.0/.

bits 16

%include "const.inc"

org loader_start

section stage1

jmp 0:start

die:
  cli
  hlt

load_readerror:
  mov si, booting_failed
  call print
  jmp die

novbe2:
  mov si, booting_novbe2
  call print
  jmp die

print:
  lodsb
  or al, al
  jz print_end
  mov ah, 0x0e
  mov bx, 9
  int 0x10
  jmp print
print_end:
  ret

start:
  mov ax, cs
  mov ds, ax
  mov es, ax
  mov ss, ax
  mov sp, loader_start

  mov [boot_disk], dl
  mov eax, [partitions+12]
  mov ebp, [stage_2_sector_count]
  sub eax, ebp
  mov [system_size], eax
  mov eax, [kernel_sector_count]
  mov [kernel_size], eax

  mov ax, 0x0007
  int 0x10

booting_msg:
  mov si, booting
  call print

check_lba:
  mov ax, 0x4100
  mov bx, 0x55AA
  int 0x13
  jc load_nolba

  mov dword [lba_packet], 0x10
  mov dword [lba_addr+4], 0x00

load_stage_2:
  mov ax, 0x4200
  mov si, lba_packet
  mov word [lba_sect_count], bp
  mov dword [lba_dst_off], stage2
  mov dword [lba_addr], 1
  int 0x13
  jc load_readerror
  jmp unreal_mode

load_nolba:
  mov byte [lba_packet], 0
  mov ah, 8
  int 0x13
  inc dh
  mov [chs_heads], dh
  and cl, 0x3f
  mov [chs_sectors], cl
  mov ax, bp
  mov ah, 0x02
  mov cx, 2
  mov dh, 0
  mov dl, [boot_disk]
  mov bx, stage2
  mov si, lba_packet
  int 0x13
  jc load_readerror

unreal_mode:
  cld
  mov eax, 0x00
  mov ecx, ((gdt_end - gdt) >> 2)
  mov edi, gdt
  rep stosd

  mov word [gdt_cs], 0xffff
  mov word [gdt_cs+5], 0xcf9a

  mov word [gdt_ds], 0xffff
  mov word [gdt_ds+5], 0xcf92

  mov word [gdt_size], (gdt_end - gdt - 1)
  mov dword [gdt_addr], gdt

  lgdt [gdt_size]

  push ds
  push es

  cli
  mov eax, cr0
  or al, 1
  mov cr0, eax
  jmp $+2 ; tell 386/486 to not crash

  mov bx, 0x10
  mov ds, bx
  mov es, bx

  and al, 0xfe
  mov cr0, eax
  jmp $+2 ; tell 386/486 to not crash

  pop es
  pop ds
  sti

get_vbe_info:
  mov di, vbe_info
  mov dword [es:di], 'VBE2'
  mov ax, 0x4f00
  int 0x10
  cmp ax, 0x004f
  jne novbe2
  cmp dword [es:di], 'VESA'
  jne novbe2

go_stage_2:
  mov si, booting_ok
  call print
  jmp stage2

section data
booting db "Loading stage 2... ",0
booting_failed db "Read error",10,13,0
booting_novbe2 db "VBE 2.0 required",10,13,0
booting_ok db "OK!",10,13,0

section system_sizes start=(loader_start + 438)
dd 0
kernel_sector_count dd 0
section partitions start=(loader_start + 446)
partitions times 4 * 16 db 0
magic dw 0xaa55
