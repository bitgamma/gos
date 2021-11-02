%ifndef CONST_ASM
%define CONST_ASM

loader_start equ 0x7c00
stage2 equ 0x7e00
kernel equ 0x100000

stage_2_sector_count equ 1

screen_width equ 800
screen_height equ 600
screen_bpp equ 8

%define disk_rb_seg (disk_rb >> 4)
%define disk_rb_off (disk_rb & 0xf)
disk_buf_size equ 64

bss_size equ 0x10000

absolute 0x500
  boot_disk resb 4 ; 4 bytes to keep alignment
  system_size resd 1

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
  scratch resb 0xC000
  stack_bottom:
    resb 0x4000
  stack_top:

absolute 0x20000
  disk_rb resb (disk_buf_size * 512)
  disk_wb resb (disk_buf_size * 512)

absolute 0x30000
  bss resb bss_size
%endif