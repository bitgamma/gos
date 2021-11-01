#ifndef __MEM__
#define __MEM__

#include <stdint.h>

#define LOADER_DATA_ADDR 0x500

#define BOOT_DISK_ADDR LOADER_DATA_ADDR
#define SYSTEM_SIZE_ADDR (LOADER_DATA_ADDR + 4)
#define LBA_TRANSFER_PACKET (SYSTEM_SIZE_ADDR + 4)
#define GDTR_ADDR (LBA_TRANSFER_PACKET + 16)
#define IDTR_ADDR (GDTR_ADDR + 6)
#define VBE_INFO_ADDR (IDTR_ADDR + 6)
#define VBE_MODE_INFO_ADDR (VBE_INFO_ADDR + 512)
#define RESERVED0 (VBE_MODE_INFO_ADDR + 256)
#define IDT_ADDR (RESERVED0 + 12)

#define BUFFER_ADDR 0x10000
#define DISK_BUF_ADDR 0x20000
#define BSS_ADDR 0x30000

#define KERNEL_DATA_ADDR 0x70000

#define TIMER_ADDR KERNEL_DATA_ADDR

#define KERNEL_GDT_ENTRY 0x08
#define SYSTEM_ADDR 0x100000

#define BOOT_DISK (*((uint32_t*)BOOT_DISK_ADDR))
#define SYSTEM_SIZE (*((uint32_t*)SYSTEM_SIZE_ADDR))

#define SYSTEM ((uint32_t*)SYSTEM_ADDR)

void memset(void* buf, uint32_t val, uint32_t count);
#endif