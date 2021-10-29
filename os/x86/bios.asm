bits 16

%include "const.inc"

section .text

global vbe_select_mode, load_system
extern _die

int13_load:
  mov ax, 0x4200
  mov dl, [boot_disk]
  push esi
  mov si, lba_packet
  int 0x13
  jc _die
  pop esi
  ret

vbe_select_mode:
	mov ax, word[vbe_info_video_modes]
	mov si, ax
	mov ax, word[vbe_info_video_modes+2]
	mov fs, ax

find_mode:
	mov cx, [fs:si]
	add si, 2

	cmp cx, 0xFFFF
	je _die

  mov ax, 0x4f01
  mov di, vbe_mode_info
  int 0x10
  cmp ax, 0x4F
  jne _die

	cmp word [vbe_mode_width], screen_width
	jne find_mode

	cmp word [vbe_mode_height], screen_height
	jne find_mode

	cmp byte [vbe_mode_bpp], screen_bpp
	jne find_mode

	mov ax, 0x4f02
	mov bx, cx
	or bx, 0x4000
	int 0x10

	cmp ax, 0x4F
  jne _die

  xor ax,ax
  mov fs, ax
	ret

load_system:
  ret