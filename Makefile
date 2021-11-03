.PHONY: run run-dosbox clean partition maxit resources

NASM=nasm
GCC=i686-elf-gcc
OBJCOPY=objcopy
DD=dd
QEMU=qemu-system-i386
DOSBOX=dosbox-x
BOCHS=bochs
PYTHON=python
CFLAGS=-std=gnu99 -ffreestanding -Os -Wall -Wextra -fomit-frame-pointer -fno-asynchronous-unwind-tables -masm=intel
LDFLAGS=-ffreestanding -nostdlib -lgcc
BUILD_DIR=build
O_DIR=$(BUILD_DIR)/o
ELF_DIR=$(BUILD_DIR)/elf
BIN_DIR=$(BUILD_DIR)/bin
SYSIMG=$(BUILD_DIR)/sys.img
APP=maxit
PALETTE=vga

INC = os/inc
SRC = os/src
APP_SRC = $(APP)/src
APP_INC = $(APP)/inc
APP_RES = $(APP)/res
BOOTLOADER = bootloader

$(shell mkdir -p $(O_DIR))
$(shell mkdir -p $(ELF_DIR))
$(shell mkdir -p $(BIN_DIR))

all: $(APP)

maxit: $(ELF_DIR)/libgos.a $(ELF_DIR)/maxit.elf partition

# Generic targets
$(O_DIR)/%.o: $(APP_SRC)/%.c
	$(GCC) $< -c $(CFLAGS) -I$(INC) -I$(APP_INC) -o $@

$(O_DIR)/%.o: $(SRC)/%.c
	$(GCC) $< -c $(CFLAGS) -I$(INC) -o $@

$(O_DIR)/interrupt.o: $(SRC)/interrupt.c
	$(GCC) $< -c $(CFLAGS) -I$(INC) -mgeneral-regs-only -o $@

$(O_DIR)/%.o: $(SRC)/%.asm
	$(NASM) $< -f elf32 -i $(INC) -i $(BOOTLOADER) -o $@

$(BIN_DIR)/%.bin: $(ELF_DIR)/%.elf
	$(OBJCOPY) -O binary -j .text $< $@

$(APP_SRC)/res.c: $(APP_RES)/*
	$(PYTHON) utils/imgc.py $@ $(APP_INC)/res.h $(APP_RES) $(BIN_DIR)/res.bin $(PALETTE).bmp

# Output programs
$(BIN_DIR)/mbr.bin: $(BOOTLOADER)/mbr.asm
	$(NASM) $< -f bin -i $(BOOTLOADER) -o $@

$(BIN_DIR)/stage2.bin: $(BOOTLOADER)/stage2.asm $(BOOTLOADER)/a20.asm $(BOOTLOADER)/bios.asm
	$(NASM) $< -f bin -i $(BOOTLOADER) -o $@

$(ELF_DIR)/maxit.elf: $(O_DIR)/startup.o $(O_DIR)/res.o $(O_DIR)/main.o
	$(GCC) $^ -o $@ -T$(INC)/app.ld $(LDFLAGS) -L$(ELF_DIR) -lgos

$(ELF_DIR)/libgos.a: $(O_DIR)/kernel.o $(O_DIR)/interrupt.o $(O_DIR)/mem.o $(O_DIR)/pic.o \
                     $(O_DIR)/pit.o $(O_DIR)/timer.o $(O_DIR)/ps2.o $(O_DIR)/keyboard.o $(O_DIR)/queue.o \
										 $(O_DIR)/2d.o $(O_DIR)/rnd.o $(O_DIR)/cmos.o
	$(AR) rcs $@ $^

$(SYSIMG): $(BIN_DIR)/mbr.bin $(BIN_DIR)/stage2.bin $(BIN_DIR)/$(APP).bin
	dd if=$(BIN_DIR)/mbr.bin of=$@ conv=sync bs=512
	dd if=$(BIN_DIR)/stage2.bin of=$@ conv=notrunc,nocreat,sync oflag=append bs=512
	dd if=$(BIN_DIR)/$(APP).bin of=$@ conv=notrunc,nocreat,sync oflag=append bs=512
	dd if=$(BIN_DIR)/res.bin of=$@ conv=notrunc,nocreat,sync oflag=append bs=512

partition: $(SYSIMG) $(BIN_DIR)/stage2.bin $(BIN_DIR)/$(APP).bin
	$(PYTHON) utils/partgen.py $^

clean:
	rm -rf $(BUILD_DIR) $(APP_SRC)/res.c $(APP_INC)/res.h

run: all
	$(QEMU) -cpu 486 -m 16M -drive file=$(SYSIMG),index=0,media=disk,format=raw

run-dosbox: all
	$(DOSBOX) -conf dosbox-x.conf

run-bochs: all
	$(BOCHS) -q