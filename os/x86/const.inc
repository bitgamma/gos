loader_start equ 0x7c00
stage2 equ 0x7e00
kernel equ 0x100000

screen_width equ 800
screen_height equ 600
screen_bpp equ 8

stage_2_sector_count equ 2

absolute 0x500
  boot_disk resb 4 ; 4 bytes to keep alignment
  system_size resd 1

  lba_packet resb 2
  lba_sect_count resw 1
  lba_dst_off resd 1
  lba_addr resd 2

  gdt_size resw 1
  gdt_addr resd 1

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

absolute 0x10000
  scratch resb 256
  disk_rb resb 32768
  disk_wb resb 32768
