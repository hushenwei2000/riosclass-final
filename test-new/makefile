# == Directories
SRC_DIR   := ./src
BUILD_DIR := ../test/rv64ui
DISASSEMBLY_DIR := build/disassembly
HEX_DIR := build/hex
# ==

# == Files
TESTS := $(shell find $(SRC_DIR) -type f -name '*.[Sc]')
TEST_BINS  := $(patsubst $(SRC_DIR)/%.S, $(BUILD_DIR)/%, $(TESTS))
# TEXTS := $(patsubst $(BUILD_DIR)/%, $(DISASSEMBLY_DIR)/%.txt, $(TEST_BINS))
# ==

# == Tools
CC      := riscv64-unknown-elf-gcc
OBJDUMP := riscv64-unknown-elf-objdump
# ==

# CFLAGS := -O0 -march=rv64i -mabi=lp64 -nostdlib -I../regression/include -Wl,-Bstatic,-T,../regression/link.ld,--strip-debug -mno-relax
CFLAGS := -O0 -march=rv64i -mabi=lp64 -nostdlib -mcmodel=medany -fvisibility=hidden -static -I./include -Wl,-Bstatic,-T,./link.ld,--strip-debug -mno-relax

.SECONDARY:
.SECONDEXPANSION:
.PHONY: build clean

clean:
	find $(BUILD_DIR) -type f | xargs rm -f

build: $(TEST_BINS)  $(BUILD_DIR) $(DISASSEMBLY_DIR)

$(BUILD_DIR)/%: $(SRC_DIR)/%.S
	mkdir -p $(BUILD_DIR)
	mkdir -p $(DISASSEMBLY_DIR)
	mkdir -p $(HEX_DIR)
	@echo "Building $@"
	$(OBJDUMP) -D $@ > $(patsubst $(BUILD_DIR)/%, $(DISASSEMBLY_DIR)/%.txt, $@)
	elf2hex 4 4096 $@ 2147483648 > $(patsubst $(BUILD_DIR)/%, $(HEX_DIR)/%.hex, $@)


