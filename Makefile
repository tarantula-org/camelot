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
TARGET  = $(OUT_DIR)/test_runner
LIB     = $(OUT_DIR)/libcamelot.a

# 3. Metadata Extraction
VERSION := $(shell grep 'CAMELOT_VERSION' include/camelot.h | cut -d '"' -f 2)

# --- PHONY TARGETS ---

.PHONY: all clean test help dirs

# 1. DEFAULT
all: test $(LIB)
	@echo " [PKG]  Generating Distribution Artifacts (v$(VERSION))..."
	@mkdir -p $(OUT_DIR)/include
	@cp -r include/* $(OUT_DIR)/include/
	
	@# Linux/Mac Script
	@echo "#!/bin/bash" > $(OUT_DIR)/install.sh
	@echo "cd \"\$$(dirname \"\$$0\")\" || exit" >> $(OUT_DIR)/install.sh
	@echo "if [ \"\$$EUID\" -ne 0 ]; then echo '[ERROR] Please run as root'; exit 1; fi" >> $(OUT_DIR)/install.sh
	@echo "echo '[INSTALL] Installing Camelot v$(VERSION) to /usr/local...'" >> $(OUT_DIR)/install.sh
	@echo "mkdir -p /usr/local/include/camelot" >> $(OUT_DIR)/install.sh
	@echo "cp -r include/* /usr/local/include/" >> $(OUT_DIR)/install.sh
	@echo "cp libcamelot.a /usr/local/lib/" >> $(OUT_DIR)/install.sh
	@echo "echo '[DONE]    Camelot v$(VERSION) installed.'" >> $(OUT_DIR)/install.sh
	@chmod +x $(OUT_DIR)/install.sh

	@# Windows Script
	@echo "@echo off" > $(OUT_DIR)/install.bat
	@echo "cd /d \"%~dp0\"" >> $(OUT_DIR)/install.bat
	@echo ":: Check for Admin Privileges" >> $(OUT_DIR)/install.bat
	@echo "NET SESSION >nul 2>&1" >> $(OUT_DIR)/install.bat
	@echo "if %errorLevel% neq 0 (" >> $(OUT_DIR)/install.bat
	@echo "    echo [ERROR] This script requires Administrator privileges." >> $(OUT_DIR)/install.bat
	@echo "    echo         Please Right-Click and select 'Run as Administrator'." >> $(OUT_DIR)/install.bat
	@echo "    pause" >> $(OUT_DIR)/install.bat
	@echo "    exit /b 1" >> $(OUT_DIR)/install.bat
	@echo ")" >> $(OUT_DIR)/install.bat
	@echo "echo [INSTALL] Installing Camelot v$(VERSION) to C:\\Camelot..." >> $(OUT_DIR)/install.bat
	@echo "if not exist C:\\Camelot\\include mkdir C:\\Camelot\\include" >> $(OUT_DIR)/install.bat
	@echo "if not exist C:\\Camelot\\lib mkdir C:\\Camelot\\lib" >> $(OUT_DIR)/install.bat
	@echo "xcopy /E /I /Y include C:\\Camelot\\include" >> $(OUT_DIR)/install.bat
	@echo "copy /Y libcamelot.a C:\\Camelot\\lib\\" >> $(OUT_DIR)/install.bat
	@echo ":: Set Environment Variables (User Scope)" >> $(OUT_DIR)/install.bat
	@echo "setx C_INCLUDE_PATH \"C:\\Camelot\\include\"" >> $(OUT_DIR)/install.bat
	@echo "setx LIBRARY_PATH \"C:\\Camelot\\lib\"" >> $(OUT_DIR)/install.bat
	@echo "echo [DONE] Camelot v$(VERSION) Setup Complete." >> $(OUT_DIR)/install.bat
	@echo "pause" >> $(OUT_DIR)/install.bat

	@echo " [PKG]  Artifacts ready at $(OUT_DIR)"

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

# 5. CLEANUP
clean:
	@echo " [RM]   Cleaning build artifacts..."
	@rm -rf $(OUT_DIR)
	@rm -f src/*/*.o

# 6. HELP
help:
	@echo "--- Camelot Build System ---"
	@echo "Available commands:"
	@echo "  make       - Run tests and build installable packages for Windows and UNIX"
	@echo "  make test  - Run the test suite"
	@echo "  make clean - Remove all build and test artifacts"
	@echo "  make help  - Show this help message"
	@echo ""
	@echo "Installation:"
	@echo "  After running 'make', execute the install script in the build output directory:"
	@echo "  - Windows: Run build\\install.bat as Administrator (Right click -> Run as Administrator)"
	@echo "  - UNIX: Run sudo ./build/install.sh"