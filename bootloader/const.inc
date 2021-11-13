%ifndef CONST_ASM
%define CONST_ASM

loader_start equ 0x7c00
stage2 equ 0x7e00
kernel equ 0x100000

stage_2_sector_count equ loader_start + 438

screen_width equ 800
screen_height equ 600
screen_bpp equ 8

logo_width equ 128
logo_height equ 128

logo_x equ (screen_width - logo_width) >> 1
logo_y equ (screen_height - logo_height) >> 1

%define disk_rb_seg (disk_rb >> 4)
%define disk_rb_off (disk_rb & 0xf)
disk_buf_size equ 127

absolute 0x500
  boot_disk resb 1
  pci_flags resb 1
  reserved resb 2
  system_size resd 1
  kernel_size resd 1

  lba_packet resb 2
  lba_sect_count resw 1
  lba_dst_off resd 1
  lba_addr resd 2

  gdt_size resw 1
  gdt_addr resd 1

  idt_size resw 1
  idt_addr resd 1

  vbe_info resb 14
  vbe_info_video_modes resd 1
  resb 494
  vbe_mode_info resb 16
	vbe_mode_pitch resw 1
	vbe_mode_width resw 1
	vbe_mode_height resw 1
  resb 3
  vbe_mode_bpp resb 1
	resb 14
  vbe_mode_fb resd 1
  vbe_mode_offscreen_off resd 1
  vbe_mode_offscreen_size resw 1
  resb 206
  gdt:
  resb 8
  gdt_cs resb 8
  gdt_ds resb 8
  resb (8 * 13)
  gdt_end:

absolute 0x10000
  disk_rb resb (disk_buf_size * 512)

%endif