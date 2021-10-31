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

load_nolba:
  mov si, booting_nolba
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
  sub eax, stage_2_sector_count
  mov [system_size], eax

  cld

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
  mov byte [lba_sect_count], stage_2_sector_count
  mov word [lba_dst_off], stage2
  mov word [lba_addr], 1
  int 0x13
  jc load_readerror

unreal_mode:
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
booting_nolba db "LBA not supported",10,13,0
booting_failed db "Read error",10,13,0
booting_novbe2 db "VBE 2.0 required",10,13,0
booting_ok db " OK!",10,13,0

gdt:
  db 0, 0, 0, 0, 0, 0, 0, 0
  db 0xff, 0xff, 0, 0, 0, 0x9a, 0xcf, 0
  db 0xff, 0xff, 0, 0, 0, 0x92, 0xcf, 0
  db 0, 0, 0, 0, 0, 0, 0, 0
  db 0, 0, 0, 0, 0, 0, 0, 0
  db 0, 0, 0, 0, 0, 0, 0, 0
  db 0, 0, 0, 0, 0, 0, 0, 0
gdt_end:

section partitions start=(loader_start + 446)
partitions times 4 * 16 db 0
magic dw 0xaa55
