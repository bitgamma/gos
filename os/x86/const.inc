loader_start equ 0x7c00
stage2 equ 0x7e00
kernel equ 0x100000

absolute 0x500
  boot_disk resb 4 ; 4 bytes to keep alignment

  lba_packet resb 2
  lba_sect_count resw 1
  lba_dst_off resd 1
  lba_addr resd 2

  gdt_size resw 1
  gdt_addr resd 1

  vbe_info resb 512

absolute 0x10000
  scratch resb 256
  disk_rb resb 8096
  disk_wb resb 8096
