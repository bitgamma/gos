loader_start equ 0x7c00
bits 16
org loader_start

section stage1

jmp 0:start

booting: db "Loading stage 2...",10,13,0

start:
  mov ax, cs
  mov ds, ax
  mov es, ax
  mov ss, ax
  mov sp, 0xfffe
  mov bp, sp

  mov ax, 0x0007
  int 0x10

  mov si, booting
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

die:
  cli
  jmp $

section partitions start=(loader_start + 446)
part1: times 16 db 0
part2: times 16 db 0
part3: times 16 db 0
part4: times 16 db 0
magic: dw 0xaa55

;section stage2 start=512
; get_vbe_info:
;   mov di, vbe_info
;   mov dword [es:di], 'VBE2'
;   mov ax, 0x4f00
;   int 0x10
;   cmp ax, 0x004f
;   jne die
;   cmp dword [es:di], 'VESA'
;   jne die

; get_mode_info:
;   mov ax, 0x4f01
;   mov cx, 0x103
;   mov di, vbe_mode_info
;   int 0x10
;   cmp ax, 0x004f
;   jne die

; set_vbe_mode:
;   mov ax, 4f02h
;   mov bx, 4103h
;   int 0x10
;   cmp ax, 004fh
;   jne die