/**
 * Copyright (c) 2021 Michele Balistreri
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef __VBE__
#define __VBE__

#include <stdint.h>
#include <mem.h>

#define VBE_INFO ((vbe_info_t *)VBE_INFO_ADDR)
#define VBE_MODE_INFO ((vbe_mode_info *)VBE_MODE_INFO_ADDR)
#define VBE_FB ((void *) VBE_MODE_INFO->framebuffer)

#define VBE_WIDTH (VBE_MODE_INFO->width)
#define VBE_HEIGHT (VBE_MODE_INFO->height)
#define VBE_BPP (VBE_MODE_INFO->bpp)
#define VBE_PIXELWIDTH (VBE_BPP >> 3)
#define VBE_PITCH (VBE_MODE_INFO->pitch)

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
} vbe_info_t;

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
} vbe_mode_info;

#endif