bits 16

%include "const.inc"

vbe_nomode_die:
  mov si, vbe_no_mode
  jmp _die

vbe_noinfo_die:
  mov si, vbe_get_info_failed
  jmp _die

vbe_setmode_die:
  mov si, vbe_set_failed
  jmp _die

lba_read_die:
  mov si, lba_read_failed
  jmp _die

vbe_select_mode:
	mov ax, word[vbe_info_video_modes]
	mov si, ax
	mov ax, word[vbe_info_video_modes+2]
	mov fs, ax

find_mode:
	mov cx, [fs:si]
	add si, 2

	cmp cx, 0xFFFF
	je vbe_nomode_die

  mov ax, 0x4f01
  mov di, vbe_mode_info
  int 0x10
  cmp ax, 0x4F
	jne vbe_noinfo_die

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
  jne vbe_set_failed

  xor ax,ax
  mov fs, ax
	ret

load_system:
  mov ebx, [system_size]
  mov edi, kernel
  mov dl, [boot_disk]
  mov word [lba_dst_off], disk_rb_off
  mov word [lba_dst_off + 2], disk_rb_seg
  mov eax, [stage_2_sector_count]
  inc eax
  mov [lba_addr], eax
  xor eax, eax

load_sectors:
  cmp ebx, disk_buf_size
  jg max_buf
  mov [lba_sect_count], bx
  jmp read
max_buf:
  mov word [lba_sect_count], disk_buf_size
read:
  mov si, lba_packet
  mov ax, 0x4200
  int 0x13
  jc lba_read_die
copy:
  mov esi, disk_rb
  mov ax, [lba_sect_count]
  mov ecx, eax
  shl ecx, 7 ; multiply by 128 (512/4)
  rep a32 movsd
next_sector:
  add [lba_addr], eax
  sub ebx, eax
  jnz load_sectors
  ret

vbe_no_mode: db "VBE mode not found",10,13,0
vbe_get_info_failed: db "VBE get mode info failed",10,13,0
vbe_set_failed: db "VBE set mode failed",10,13,0
lba_read_failed: db "LBA read failed",10,13,0