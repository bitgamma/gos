/**
 * Copyright (c) 2021 Michele Balistreri
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef __PCI__
#define __PCI__

#include <stdint.h>
#include <port.h>
#include <mem.h>

#define PCI_CONFIG_ADDRESS_PORT 0xcf8
#define PCI_CONFIG_DATA_PORT 0xcfc

#define PCI_VENDOR_INVALID 0xffff
#define PCI_VENDOR(a) a & 0xffff
#define PCI_DEVID(a) (a >> 16)
#define PCI_CLASS(a) (a >> 16)
#define PCI_HEADER_TYPE(a) ((a >> 16) & 0xff)
#define PCI_MULTIFUNCTION(a) ((a >> 16) & 0x80)
#define PCI_SECONDARY_BUS(a) ((a >> 8) & 0xff)

#define PCI_HEADER_DEV_VENDORID 0x00
#define PCI_HEADER_STATUS_COMMAND 0x04
#define PCI_HEADER_CLASS 0x08
#define PCI_HEADER_INFO 0xc
#define PCI_HEADER_BAR0 0x10
#define PCI_HEADER_BAR1 0x14
#define PCI_HEADER_BAR2 0x18
#define PCI_HEADER_BAR3 0x1c
#define PCI_HEADER_BAR4 0x20
#define PCI_HEADER_BAR5 0x24

#define PCI_HEADER_SECONDARY 0x18

#define PCI_PCI_BRIDGE_CLASS 0x0604
#define PCI_AC97_CLASS 0x0401
#define PCI_HDAUDIO_CLASS 0x0403

#define PCI_COMMAND_PIO 0x01
#define PCI_COMMAND_MMIO 0x02
#define PCI_COMMAND_BUSMASTER 0x04

#define AC97_DEV (*((pci_addr_t*)AC97_DEV_ADDR))
#define HDAUDIO_DEV (*((pci_addr_t*)AC97_DEV_ADDR))

typedef uint32_t pci_addr_t;

void pci_scan();

inline uint32_t pci_config_read(pci_addr_t addr, uint8_t offset) {
  outd(PCI_CONFIG_ADDRESS_PORT, (addr | offset));
  return ind(PCI_CONFIG_DATA_PORT);
}

inline void pci_config_write(pci_addr_t addr, uint8_t offset, uint32_t data) {
  outd(PCI_CONFIG_ADDRESS_PORT, (addr | offset));
  outd(PCI_CONFIG_DATA_PORT, data);
}

__attribute__((always_inline)) inline void pci_config_or(pci_addr_t addr, uint8_t offset, uint32_t data) {
  pci_config_write(addr, offset, pci_config_read(addr, offset) | data);
}

#endif