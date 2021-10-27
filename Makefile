.PHONY: run clean

NASM=nasm
GCC=i686-elf-gcc
OBJCOPY=objcopy
DD=dd
CFLAGS=-std=gnu99 -ffreestanding -Os -Wall -Wextra
LDFLAGS=-ffreestanding -nostdlib -lgcc
BUILD_DIR=build
O_DIR=$(BUILD_DIR)/o
ELF_DIR=$(BUILD_DIR)/elf
BIN_DIR=$(BUILD_DIR)/bin
SYSIMG=$(BUILD_DIR)/sys.img

X86_SRC = os/x86

$(shell mkdir -p $(O_DIR))
$(shell mkdir -p $(ELF_DIR))
$(shell mkdir -p $(BIN_DIR))

all: $(SYSIMG)

# Generic targets
$(O_DIR)/%.o: $(X86_SRC)/%.c
	$(GCC) $< -c $(CFLAGS) -o $@

$(O_DIR)/%.o: $(X86_SRC)/%.asm
	$(NASM) $< -f elf32 -o $@

$(BIN_DIR)/%.bin: $(ELF_DIR)/%.elf
	$(OBJCOPY) -O binary -j .text $< $@

# Output programs
$(BIN_DIR)/mbr.bin: $(X86_SRC)/mbr.asm
	$(NASM) $< -f bin -o $@

$(ELF_DIR)/stage2.elf: $(O_DIR)/stage2.o
	$(GCC) $< -o $@ -T$(X86_SRC)/stage2.ld $(LDFLAGS)

$(SYSIMG): $(BIN_DIR)/mbr.bin $(BIN_DIR)/stage2.bin
	cp $(BIN_DIR)/mbr.bin $@
	dd if=$(BIN_DIR)/stage2.bin of=$@ conv=notrunc,nocreat,sync oflag=append bs=512

clean:
	rm -rf $(BUILD_DIR)

run: all
	qemu-system-i386 -cpu 486 -m 16M -drive file=$(SYSIMG),index=0,media=disk,format=raw