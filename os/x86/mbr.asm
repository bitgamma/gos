loader_start equ 0x7c00
bits 16
org loader_start

section stage1

entry: jmp 0:start

booting: db "Loading stage 2...",0
booting_nolba: db " LBA not supported",10,13,0
booting_failed: db " Failed!",10,13,0
booting_ok: db " OK!",10,13,0

boot_disk: db 0

align 4
lba_packet: db 0x10
db 0x00
lba_sect_count: db 0x00
db 0x00
lba_dst_off: dw 0x0000
dw 0x0000
lba_16_bit: dw 0x0000
dw 0x0000
dd 0x00000000

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

  mov si, booting
  call print

  mov ax, 0x4100
  mov bx, 0x55AA
  int 0x13
  jc load_nolba

  mov ax, 0x4200
  mov si, lba_packet
  mov byte [lba_sect_count], 2
  mov word [lba_dst_off], stage2
  mov word [lba_16_bit], 1
  int 0x13
  jc load_fail

  mov si, booting_ok
  call print
  jmp stage2

section partitions start=(loader_start + 446)
part1: times 16 db 0
part2: times 16 db 0
part3: times 16 db 0
part4: times 16 db 0
magic: dw 0xaa55

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

vbe_info: times 512 db 0
vbe_mode_info: times 256 db 0
