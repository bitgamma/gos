.PHONY: run run-dosbox clean partition

NASM=nasm
GCC=i686-elf-gcc
OBJCOPY=objcopy
DD=dd
QEMU=qemu-system-i386
DOSBOX=D:\Emulation\dosbox-x\dosbox-x.exe
BOCHS=bochs
PYTHON=python
CFLAGS=-std=gnu99 -ffreestanding -Os -Wall -Wextra -fno-zero-initialized-in-bss -fomit-frame-pointer
LDFLAGS=-ffreestanding -nostdlib -lgcc
BUILD_DIR=build
O_DIR=$(BUILD_DIR)/o
ELF_DIR=$(BUILD_DIR)/elf
BIN_DIR=$(BUILD_DIR)/bin
SYSIMG=$(BUILD_DIR)/sys.img

INC = os/inc
SRC = os/src
RES = os/res
X86_SRC = os/x86

$(shell mkdir -p $(O_DIR))
$(shell mkdir -p $(ELF_DIR))
$(shell mkdir -p $(BIN_DIR))

all: partition

# Generic targets
$(O_DIR)/%.o: $(SRC)/%.c
	$(GCC) $< -c $(CFLAGS) -I$(INC) -I$(X86_SRC) -o $@

$(O_DIR)/%.o: $(X86_SRC)/%.c
	$(GCC) $< -c $(CFLAGS) -I$(INC) -I$(X86_SRC) -o $@

$(O_DIR)/%.o: $(X86_SRC)/%.asm
	$(NASM) $< -f elf32 -i $(X86_SRC) -o $@

$(BIN_DIR)/%.bin: $(ELF_DIR)/%.elf
	$(OBJCOPY) -O binary -j .text $< $@

# Output programs
$(BIN_DIR)/mbr.bin: $(X86_SRC)/mbr.asm
	$(NASM) $< -f bin -i $(X86_SRC) -o $@

$(BIN_DIR)/stage2.bin: $(X86_SRC)/stage2.asm $(X86_SRC)/a20.asm $(X86_SRC)/bios.asm
	$(NASM) $< -f bin -i $(X86_SRC) -o $@

$(ELF_DIR)/kernel.elf: $(O_DIR)/kernel.o
	$(GCC) $^ -o $@ -T$(SRC)/kernel.ld $(LDFLAGS)

$(SYSIMG): $(BIN_DIR)/mbr.bin $(BIN_DIR)/stage2.bin $(BIN_DIR)/kernel.bin
	dd if=$(BIN_DIR)/mbr.bin of=$@ conv=sync bs=512
	dd if=$(BIN_DIR)/stage2.bin of=$@ conv=notrunc,nocreat,sync oflag=append bs=512
	dd if=$(BIN_DIR)/kernel.bin of=$@ conv=notrunc,nocreat,sync oflag=append bs=512
	dd if=$(RES)/loading.bmp of=$@ conv=notrunc,nocreat,sync oflag=append bs=512

partition: $(SYSIMG)
	$(PYTHON) utils/partgen.py $<

clean:
	rm -rf $(BUILD_DIR)

run: all
	$(QEMU) -cpu 486 -m 16M -drive file=$(SYSIMG),index=0,media=disk,format=raw

run-dosbox: all
	$(DOSBOX) -conf dosbox-x.conf

run-bochs: all
	$(BOCHS) -q