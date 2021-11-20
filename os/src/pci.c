/**
 * Copyright (c) 2021 Michele Balistreri
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <pci.h>

static void pci_scan_device(uint8_t bus, uint8_t device);
static void pci_scan_bus(uint8_t bus);

static inline pci_addr_t pci_addr(uint32_t bus, uint32_t slot, uint32_t func) {
  return (1 << 31) | (bus << 16) | (slot << 11) | (func << 8);
}

static inline uint16_t pci_vendor_id(uint8_t bus, uint8_t slot, uint8_t func) {
  return PCI_VENDOR(pci_config_read(pci_addr(bus, slot, func), PCI_HEADER_DEV_VENDORID));
}

static inline uint16_t pci_is_multifunction(uint8_t bus, uint8_t slot, uint8_t func) {
  return PCI_MULTIFUNCTION(pci_config_read(pci_addr(bus, slot, func), PCI_HEADER_INFO));
}

static inline uint16_t pci_class(uint8_t bus, uint8_t slot, uint8_t func) {
  return PCI_CLASS(pci_config_read(pci_addr(bus, slot, func), PCI_HEADER_CLASS));
}

static inline uint16_t pci_secondary_bus(uint8_t bus, uint8_t slot, uint8_t func) {
  return PCI_SECONDARY_BUS(pci_config_read(pci_addr(bus, slot, func), PCI_HEADER_SECONDARY));
}

static void pci_scan_function(uint8_t bus, uint8_t device, uint8_t function) {
  // for now we only ever look for sound cards. In the future we either
  // define statically what device we are interested in or we save all we found
  uint16_t class = pci_class(bus, device, function);

  switch(class) {
    case PCI_PCI_BRIDGE_CLASS:
      pci_scan_bus(pci_secondary_bus(bus, device, function));
      break;
    case PCI_AC97_CLASS:
      AC97_DEV = pci_addr(bus, device, function);
      break;
    case PCI_HDAUDIO_CLASS:
      HDAUDIO_DEV = pci_addr(bus, device, function);
      break;
  }
}

static void pci_scan_device(uint8_t bus, uint8_t device) {
  if (pci_vendor_id(bus, device, 0) == PCI_VENDOR_INVALID) {
    return;
  }

  pci_scan_function(bus, device, 0);

  if (pci_is_multifunction(bus, device, 0)) {
    for (uint8_t function = 1; function < 8; function++) {
      if (pci_vendor_id(bus, device, function) != PCI_VENDOR_INVALID) {
        pci_scan_function(bus, device, function);
      }
    }
  }
}

static void pci_scan_bus(uint8_t bus) {
  for (uint8_t device = 0; device < 32; device++) {
    pci_scan_device(bus, device);
  }
}

void pci_scan() {
  if ((PCI_FLAGS & 0x01) != 0x01) {
    AC97_DEV = 0;
    return;
  }

  if (pci_is_multifunction(0, 0, 0)) {
    for (uint8_t function = 0; function < 8; function++) {
      if (pci_vendor_id(0, 0, function) != PCI_VENDOR_INVALID) {
        break;
      }

      pci_scan_bus(function);
    }
  } else {
    pci_scan_bus(0);
  }
}
