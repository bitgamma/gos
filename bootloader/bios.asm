; Copyright (c) 2021 Michele Balistreri
;
; This Source Code Form is subject to the terms of the Mozilla Public
; License, v. 2.0. If a copy of the MPL was not distributed with this
; file, You can obtain one at http://mozilla.org/MPL/2.0/.

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

read_die:
  mov si, read_failed
  jmp _die

vbe_select_mode:
	mov ax, word[vbe_info_video_modes]
	mov si, ax
	mov ax, word[vbe_info_video_modes+2]
	mov fs, ax

find_mode:
	mov cx, [fs:si]
	add si, 2

	cmp cx, 0xffff
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

	cmp ax, 0x4f
  jne vbe_setmode_die

  xor ax,ax
  mov fs, ax
	ret

load_system:
  mov ebp, [system_size]
  mov eax, [stage_2_sector_count]
  inc eax
  mov edi, kernel
  mov cl, [lba_packet]
  or cl, cl
  jz load_chs
load_lba:
  mov dl, [boot_disk]
  mov word [lba_dst_off], disk_rb_off
  mov word [lba_dst_off + 2], disk_rb_seg
  mov [lba_addr], eax
  xor eax, eax
load_sectors:
  cmp bp, disk_buf_size
  jg max_buf
  mov [lba_sect_count], bp
  jmp read
max_buf:
  mov word [lba_sect_count], disk_buf_size
read:
  mov si, lba_packet
  mov ax, 0x4200
  int 0x13
  jc read_die
copy:
  mov esi, disk_rb
  mov ax, [lba_sect_count]
  mov ecx, eax
  shl ecx, 7 ; multiply by 128 (512/4)
  rep a32 movsd
next_sector:
  add [lba_addr], eax
  sub ebp, eax
  jnz load_sectors
  ret
load_chs:
  mov bl, [chs_sectors]
  div bl
  mov cl, ah
  xor ah, ah
  mov bl, [chs_heads]
  div bl
  mov dh, ah
  mov [chs_head], dh
  xor ah, ah
  mov [chs_cylinder], ax
  mov ch, al
  mov al, [chs_sectors]
  sub al, cl
  inc cl
  mov dl, [boot_disk]
  jmp chs_read
load_track:
  mov dh, [chs_head]
  mov cx, [chs_cylinder]
  xchg cl, ch
  shl cl, 6
  or cl, 1
  mov al, [chs_sectors]
  cmp bp, ax
  jg chs_read
  mov ax, bp
chs_read:
  mov ah, 2
  mov bx, disk_rb_seg
  mov es, bx
  mov bx, disk_rb_off
  mov [chs_tmp], al
  int 0x13
  jc read_die
  mov al, [chs_tmp]
  mov esi, disk_rb
  mov bx, ds
  mov es, bx
  mov ecx, eax
  shl ecx, 7 ; multiply by 128 (512/4)
  rep a32 movsd
next_head:
  mov cl, [chs_head]
  inc cl
  cmp cl, [chs_heads]
  jne next_track
  mov cl, 0
  inc word [chs_cylinder]
next_track:
  mov [chs_head], cl
  sub bp, ax
  jnz load_track
  ret

vbe_no_mode: db "VBE mode not found",10,13,0
vbe_get_info_failed: db "VBE get mode info failed",10,13,0
vbe_set_failed: db "VBE set mode failed",10,13,0
read_failed: db "Read failed",10,13,0