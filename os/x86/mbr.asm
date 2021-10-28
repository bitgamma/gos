loader_start equ 0x7c00
bits 16
org loader_start

absolute 0x500
  boot_disk resb 4 ; 4 bytes to keep alignment

  lba_packet resb 2
  lba_sect_count resw 1
  lba_dst_off resd 1
  lba_addr resd 2

  gdt_size resw 1
  gdt_addr resd 1

  vbe_info resb 512
  vbe_mode_info resb 256

section stage1

jmp 0:start

die:
  cli
  jmp $

load_fail:
  mov si, booting_failed
  call print
  jmp die

load_nolba:
  mov si, booting_nolba
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
  mov sp, 0x7C00
  mov bp, sp
  mov [boot_disk], dl

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
  mov dword [lba_dst_off], 0x00
  mov dword [lba_addr], 0x00
  mov dword [lba_addr+4], 0x00

load_stage_2:
  mov ax, 0x4200
  mov si, lba_packet
  mov byte [lba_sect_count], 2
  mov word [lba_dst_off], stage2
  mov word [lba_addr], 1
  int 0x13
  jc load_fail

unreal_mode:
  mov word [gdt_size], (gdt_end - gdt - 1)
  mov dword [gdt_addr], gdt

  cli
  push ds
  push es
  lgdt [gdt_size]

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

go_stage_2:
  mov si, booting_ok
  call print
  jmp stage2

section data
booting db "Loading stage 2...",0
booting_nolba db " LBA not supported",10,13,0
booting_failed db " Failed!",10,13,0
booting_ok db " OK!",10,13,0

gdt:
  db 0, 0, 0, 0, 0, 0, 0, 0
  db 0xff, 0xff, 0, 0, 0, 10011010b, 10001111b, 0
  db 0xff, 0xff, 0, 0, 0, 10010010b, 11001111b, 0
gdt_end:

section partitions start=(loader_start + 446)
part1: times 16 db 0
part2: times 16 db 0
part3: times 16 db 0
part4: times 16 db 0
magic dw 0xaa55

section stage2 start=(loader_start + 512)
stage2:
get_vbe_info:
  mov di, vbe_info
  mov dword [es:di], 'VBE2'
  mov ax, 0x4f00
  int 0x10
  cmp ax, 0x004f
  jne die
  cmp dword [es:di], 'VESA'
  jne die

get_mode_info:
  mov ax, 0x4f01
  mov cx, 0x103
  mov di, vbe_mode_info
  int 0x10
  cmp ax, 0x004f
  jne die

set_vbe_mode:
  mov ax, 4f02h
  mov bx, 4103h
  int 0x10
  cmp ax, 004fh
  jne die
