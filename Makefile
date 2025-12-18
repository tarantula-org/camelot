# --- CAMELOT BUILD SYSTEM ---

CC      = gcc
AR      = ar
CFLAGS  = -I include -Wall -Wextra -std=c2x -Wno-unused-function

# VERSIONING
VERSION := $(shell grep 'define CAMELOT_VERSION' include/camelot/camelot.h | cut -d '"' -f 2)

# 1. Source Directories
SRC_DIR = src
SRCS    = $(wildcard src/memory/*.c) \
          $(wildcard src/types/*.c) \
          $(wildcard src/io/*.c) \
          $(wildcard src/ds/*.c)

# 2. Object Files
OBJS    = $(SRCS:.c=.o)

# 3. Output Configuration
OUT_DIR = packages
DIST    = $(OUT_DIR)/dist
TARGET  = $(OUT_DIR)/test_runner
LIB     = $(DIST)/lib/libcamelot.a

# --- RULES ---

.PHONY: all clean test package dirs

all: package

dirs:
	@mkdir -p $(OUT_DIR)
	@mkdir -p $(DIST)/lib
	@mkdir -p $(DIST)/include

# --- TEST SUITE ---
test: dirs
	@echo " [INFO] Camelot v$(VERSION)"
	@echo " [CC]   Compiling Test Suite..."
	@$(CC) $(CFLAGS) tests/main.c $(SRCS) -o $(TARGET)
	@echo " [EXEC] Running Tests..."
	@echo "--------------------------------------------------"
	@./$(TARGET)

# --- PACKAGE GENERATION ---

%.o: %.c
	@echo " [CC]   $<"
	@$(CC) $(CFLAGS) -c $< -o $@

$(LIB): dirs $(OBJS)
	@echo " [AR]   Creating Static Library..."
	@$(AR) rcs $(LIB) $(OBJS)
	@rm -f $(OBJS)

package: $(LIB)
	@echo " [INFO] Building Camelot v$(VERSION)"
	@echo " [CP]   Copying Headers..."
	@cp -r include/* $(DIST)/include/
	
	@echo " [GEN]  Generating Installers..."
	
	# 1. Linux/macOS Installer
	@echo "#!/bin/bash" > $(DIST)/install.sh
	@echo "if [ \"\$$EUID\" -ne 0 ]; then echo 'Please run as root'; exit; fi" >> $(DIST)/install.sh
	@echo "echo '[*] Installing Camelot v$(VERSION)...'" >> $(DIST)/install.sh
	
	# --- FIX START ---
	# Copy ALL directories (camelot, ds, types) to /usr/local/include/
	@echo "cp -r include/* /usr/local/include/" >> $(DIST)/install.sh
	# --- FIX END ---
	
	@echo "cp lib/libcamelot.a /usr/local/lib/" >> $(DIST)/install.sh
	@echo "echo '[V] Camelot Installed! Link with: -lcamelot'" >> $(DIST)/install.sh
	@chmod +x $(DIST)/install.sh
	
	# 2. Windows Installer
	@echo "@echo off" > $(DIST)/install.bat
	@echo "echo [*] Installing Camelot v$(VERSION) to C:\\Camelot..." >> $(DIST)/install.bat
	
	@echo "if not exist \"C:\\Camelot\" mkdir \"C:\\Camelot\"" >> $(DIST)/install.bat
	# (Windows xcopy /E already handles subfolders correctly)
	@echo "xcopy /E /Y include \"C:\\Camelot\\include\\\" >NUL" >> $(DIST)/install.bat
	@echo "xcopy /E /Y lib \"C:\\Camelot\\lib\\\" >NUL" >> $(DIST)/install.bat
	
	@echo "echo [*] Registering Environment Variables..." >> $(DIST)/install.bat
	@echo "setx C_INCLUDE_PATH \"C:\\Camelot\\include\" >NUL" >> $(DIST)/install.bat
	@echo "setx LIBRARY_PATH \"C:\\Camelot\\lib\" >NUL" >> $(DIST)/install.bat
	
	@echo "echo [V] Installation Complete!" >> $(DIST)/install.bat
	@echo "pause" >> $(DIST)/install.bat

	@echo " [DONE] Package v$(VERSION) ready at: $(DIST)/"

clean:
	@echo " [RM]   Cleaning artifacts..."
	@rm -rf $(OUT_DIR)
	@rm -f src/*/*.o

clean:
	@echo " [RM]   Cleaning artifacts..."
	@rm -rf $(OUT_DIR)
	@rm -f src/*/*.o