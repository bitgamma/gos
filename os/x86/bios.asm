bits 16

%include "const.inc"

section .text

global vbe_select_mode, load_system
extern _die

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

  and byte [vbe_mode_info], 0x80
  jz find_mode

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
  xor eax,eax
  mov ebx, [system_size]
  mov dl, [boot_disk]
  mov si, lba_packet
  mov word [lba_dst_off], disk_rb_off
  mov word [lba_dst_off + 2], disk_rb_seg
  mov dword [lba_addr], (stage_2_sector_count + 1)

load_sectors:
  cmp ebx, disk_buf_size
  jg max_buf
  mov [lba_sect_count], bx
  jmp read
max_buf:
  mov word [lba_sect_count], disk_buf_size
read:
  mov ax, 0x4200
  int 0x13
  jc _die

  mov ax, [lba_sect_count]
  add [lba_addr], eax
  sub ebx, eax
  jnz load_sectors
  ret