#ifndef __MEM__
#define __MEM__

#include <stdint.h>

#define DATA_BASE_ADDR 0x500
#define SYSTEM_SIZE_ADDR (DATA_BASE_ADDR + 4)
#define VBE_INFO_ADDR (DATA_BASE_ADDR + 30)
#define VBE_MODE_INFO_ADDR ((VBE_INFO_ADDR) + 512))

#define BUFFER_ADDR 0x10000
#define SCRATCH_BUF_ADDR BUFFER_ADDR
#define DISK_READ_BUF_ADDR (BUFFER_ADDR + 256)

#define SYSTEM_ADDR 0x100000

#define SYSTEM_SIZE (*((uint32_t*)SYSTEM_SIZE_ADDR))
#define SYSTEM ((uint32_t*)SYSTEM_ADDR)

#endif