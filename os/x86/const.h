#ifndef __CONST__
#define __CONST__

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
#define VBE_INFO ((VBE_Info *)VBE_INFO_ADDR)
#define VBE_MODE_INFO ((VBE_Mode_Info *)(VBE_MODE_INFO_ADDR)

typedef struct __attribute__ ((packed)) {
	uint32_t identifer;
	uint16_t version;
	uint32_t oem;
	uint32_t capabilities;
	uint32_t video_modes;
	uint16_t video_memory;
	uint16_t software_rev;
	uint32_t vendor;
	uint32_t product_name;
	uint32_t product_rev;
	uint8_t reserved[222];
	uint8_t oem_data[256];
} VBE_Info;

typedef struct __attribute__ ((packed)) {
	uint16_t attributes;
	uint8_t window_a;
	uint8_t window_b;
	uint16_t granularity;
	uint16_t window_size;
	uint16_t segment_a;
	uint16_t segment_b;
	uint32_t win_func_ptr;
	uint16_t pitch;
	uint16_t width;
	uint16_t height;
	uint8_t w_char;
	uint8_t y_char;
	uint8_t planes;
	uint8_t bpp;
	uint8_t banks;
	uint8_t memory_model;
	uint8_t bank_size;
	uint8_t image_pages;
	uint8_t reserved0;

	uint8_t red_mask;
	uint8_t red_position;
	uint8_t green_mask;
	uint8_t green_position;
	uint8_t blue_mask;
	uint8_t blue_position;
	uint8_t reserved_mask;
	uint8_t reserved_position;
	uint8_t direct_color_attributes;

	uint32_t framebuffer;
	uint32_t off_screen_mem_off;
	uint16_t off_screen_mem_size;
	uint8_t reserved1[206];
} VBE_Mode_Info;

#endif