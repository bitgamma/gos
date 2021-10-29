#ifndef __CONST__
#define __CONST__

#include <stdint.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define SCREEN_BPP 8

#define DATA_BASE_ADDR 0x500
#define VBE_INFO_ADDR (DATA_BASE_ADDR + 30)
#define VBE_MODE_INFO_ADDR ((VBE_INFO_ADDR) + 512))

#define VBE_INFO ((VBE_Info *)VBE_INFO_ADDR)
#define VBE_MODE_INFO ((VBE_Mode_Info *)(VBE_MODE_INFO_ADDR)
#define VBE_EOL 0xffff
#define VBE_FB_MASK 0x80
#define LINEAR_ADDR(a) (((a >> 16) << 4) + (a & 0xff))

extern void vbe_get_mode_info(uint16_t mode);
extern void vbe_set_mode(uint16_t mode);

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