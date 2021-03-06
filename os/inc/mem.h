/**
 * Copyright (c) 2021 Michele Balistreri
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef __MEM__
#define __MEM__

#include <stdint.h>
#include <config.h>

#define BIOS_DATA_ADDR 0x400

#define LOADER_DATA_ADDR 0x500

#define BOOT_DISK_ADDR LOADER_DATA_ADDR
#define PCI_FLAGS_ADDR (LOADER_DATA_ADDR + 1)
#define LOADER_RESERVED (PCI_FLAGS_ADDR + 1)
#define SYSTEM_SIZE_ADDR (LOADER_RESERVED + 2)
#define KERNEL_SIZE_ADDR (SYSTEM_SIZE_ADDR + 4)
#define LBA_TRANSFER_PACKET (KERNEL_SIZE_ADDR + 4)
#define GDTR_ADDR (LBA_TRANSFER_PACKET + 16)
#define IDTR_ADDR (GDTR_ADDR + 6)
#define VBE_INFO_ADDR (IDTR_ADDR + 6)
#define VBE_MODE_INFO_ADDR (VBE_INFO_ADDR + 512)
#define GDT_ADDR (VBE_MODE_INFO_ADDR + 256)
#define SYSTICK_ADDR (GDT_ADDR + 128)
#define IDT_ADDR (SYSTICK_ADDR + 8)
#define AC97_DEV_ADDR (IDT_ADDR + 2048)
#define HDAUDIO_DEV_ADDR (AC97_DEV_ADDR + 4)
#define DMA_BUFFER_ADDR (HDAUDIO_DEV_ADDR + 4)

#ifndef DMA_BUFFER_SIZE
#define DMA_BUFFER_SIZE 32768
#endif
#define SYSTEM_ADDR 0x100000

#define BOOT_DISK (*((uint8_t*)BOOT_DISK_ADDR))
#define PCI_FLAGS (*((uint8_t*)PCI_FLAGS_ADDR))
#define SYSTEM_SIZE (*((uint32_t*)SYSTEM_SIZE_ADDR))
#define KERNEL_SIZE (*((uint32_t*)KERNEL_SIZE_ADDR))
#define SYSTICK (*((uint32_t*)SYSTICK_ADDR))

#define SYSTEM ((uint32_t*)SYSTEM_ADDR)

#define NULL ((void*)0)

void memset(void* dst, uint8_t val, uint32_t size);
void memcpy(void* dst, void* src, uint32_t size);

inline void memset32(uint32_t* dst, uint32_t val, uint32_t count) {
  asm volatile (
    "cld\n"
    "rep\n"
    "stosd"
    : "=D" (dst), "=c" (count) : "0" (dst), "1" (count), "a" (val)
    : "memory");
}

inline void memcpy32(uint32_t* dst, uint32_t *src, uint32_t count) {
  asm volatile (
    "cld\n"
    "rep\n"
    "movsd"
    : "=D" (dst), "=S" (src), "=c" (count) : "0" (dst), "1" (src), "2" (count)
    : "memory");
}

#endif