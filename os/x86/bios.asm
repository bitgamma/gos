bits 16

%include "const.inc"

section .text

global vbe_get_mode_info, vbe_set_mode, int13_load
extern _die

vbe_get_mode_info:
  mov ecx, [esp+4]
  mov ax, 0x4f01
  push edi
  mov di, vbe_mode_info
  int 0x10
  pop edi
  ret

vbe_set_mode:
  mov ecx, ebx
  mov ebx, [esp+4]
  or bx, 0x4000
  mov ax, 0x4f02
  int 0x10
  cmp ax, 0x004f
  jne _die
  mov ebx, ecx
  ret

int13_load:
  mov ax, 0x4200
  mov dl, [boot_disk]
  push esi
  mov si, lba_packet
  int 0x13
  jc _die
  pop esi
  ret