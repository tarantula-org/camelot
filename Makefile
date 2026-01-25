# --- CAMELOT BUILD SYSTEM ---

CC      = gcc
AR      = ar
CFLAGS  = -I include -Wall -Wextra -std=c2x -Wno-unused-function

# 1. Source Directories
SRCS    = $(wildcard src/*/*.c)
TEST_SRCS = $(wildcard tests/*.c)

# 2. Artifacts
OBJS    = $(SRCS:.c=.o)
OUT_DIR = build
PKG_DIR = packages
TARGET  = $(OUT_DIR)/test_runner
LIB     = $(OUT_DIR)/libcamelot.a

# 3. Metadata Extraction
VERSION := $(shell grep 'CAMELOT_VERSION' include/camelot.h | cut -d '"' -f 2)

# --- PHONY TARGETS ---

.PHONY: all clean test dirs dist

# 1. DEFAULT
all: $(LIB)

# 2. INTERNAL UTILITY
dirs:
	@mkdir -p $(OUT_DIR)

# 3. TEST SUITE
test: dirs
	@echo " [CC]   Compiling Test Suite..."
	@$(CC) $(CFLAGS) $(TEST_SRCS) $(SRCS) -o $(TARGET)
	@echo " [EXEC] Running Tests..."
	@./$(TARGET)

# 4. LIBRARY GENERATION
%.o: %.c
	@echo " [CC]   $<"
	@$(CC) $(CFLAGS) -c $< -o $@

$(LIB): dirs $(OBJS)
	@echo " [AR]   Creating Static Library..."
	@$(AR) rcs $(LIB) $(OBJS)
	@rm -f $(OBJS)

# 5. DISTRIBUTION
dist: $(LIB)
	@echo " [PKG]  Cleaning old packages..."
	@rm -rf $(PKG_DIR)
	@echo " [PKG]  Generating Distribution Artifacts (v$(VERSION))..."
	@mkdir -p $(PKG_DIR)/dist/include
	@mkdir -p $(PKG_DIR)/dist/lib
	@cp -r include/* $(PKG_DIR)/dist/include/
	@cp $(LIB) $(PKG_DIR)/dist/lib/
	
	@# Linux/Mac Script
	@echo "#!/bin/bash" > $(PKG_DIR)/dist/install.sh
	@echo "echo '[INSTALL] Installing Camelot v$(VERSION) to /usr/local...'" >> $(PKG_DIR)/dist/install.sh
	@echo "sudo cp -r include/* /usr/local/include/" >> $(PKG_DIR)/dist/install.sh
	@echo "sudo cp lib/libcamelot.a /usr/local/lib/" >> $(PKG_DIR)/dist/install.sh
	@echo "echo '[DONE]    Camelot v$(VERSION) installed.'" >> $(PKG_DIR)/dist/install.sh
	@chmod +x $(PKG_DIR)/dist/install.sh

	@# Windows Script
	@echo "@echo off" > $(PKG_DIR)/dist/install.bat
	@echo "echo [INSTALL] Installing Camelot v$(VERSION) to C:\\Camelot..." >> $(PKG_DIR)/dist/install.bat
	@echo "mkdir C:\\Camelot\\include" >> $(PKG_DIR)/dist/install.bat
	@echo "mkdir C:\\Camelot\\lib" >> $(PKG_DIR)/dist/install.bat
	@echo "xcopy /E /I /Y include C:\\Camelot\\include" >> $(PKG_DIR)/dist/install.bat
	@echo "copy /Y lib\\libcamelot.a C:\\Camelot\\lib\\" >> $(PKG_DIR)/dist/install.bat
	@echo "setx C_INCLUDE_PATH \"C:\\Camelot\\include\"" >> $(PKG_DIR)/dist/install.bat
	@echo "setx LIBRARY_PATH \"C:\\Camelot\\lib\"" >> $(PKG_DIR)/dist/install.bat
	@echo "echo [DONE] Camelot v$(VERSION) Setup Complete." >> $(PKG_DIR)/dist/install.bat

	@echo " [PKG]  Artifacts ready at $(PKG_DIR)/dist"

# 6. CLEANUP
clean:
	@echo " [RM]   Cleaning build and package artifacts..."
	@rm -rf $(OUT_DIR)
	@rm -rf $(PKG_DIR)
	@rm -f src/*/*.o