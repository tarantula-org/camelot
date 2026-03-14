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

# 3. OS Detection & Metadata
ifeq ($(OS),Windows_NT)
    # Windows
    VERSION := $(shell powershell -noprofile -command "(Get-Content include\camelot.h | Select-String CAMELOT_VERSION).ToString().Split([char]34)[1]")
    RM := cmd /c del /f /q
    RMDIR := cmd /c rmdir /s /q
    MKDIR := cmd /c mkdir
    RM_OBJS := $(RM) $(subst /,\,$(OBJS))
    TEST_TARGET := $(TARGET).exe
    INSTALL_SCRIPT := install_win
    DEV_NULL := NUL
    TRUE_CMD := ver > NUL
    CP_INC := xcopy /E /I /Y include $(OUT_DIR)\include >$(DEV_NULL)
    MKDIR_INC := $(MKDIR) $(OUT_DIR)\include 2>$(DEV_NULL) || $(TRUE_CMD)
    MKDIR_BUILD := $(MKDIR) $(OUT_DIR) 2>$(DEV_NULL) || $(TRUE_CMD)
else
    # Linux / macOS
    VERSION := $(shell grep 'CAMELOT_VERSION' include/camelot.h | cut -d '"' -f 2)
    RM := rm -f
    RMDIR := rm -rf
    MKDIR := mkdir -p
    RM_OBJS := $(RM) $(OBJS)
    TEST_TARGET := $(TARGET)
    INSTALL_SCRIPT := install_unix
    DEV_NULL := /dev/null
    TRUE_CMD := true
    CP_INC := cp -r include/* $(OUT_DIR)/include/
    MKDIR_INC := $(MKDIR) $(OUT_DIR)/include 2>$(DEV_NULL) || $(TRUE_CMD)
    MKDIR_BUILD := $(MKDIR) $(OUT_DIR) 2>$(DEV_NULL) || $(TRUE_CMD)
endif

# --- PHONY TARGETS ---

.PHONY: all clean test help dirs install_script install_win install_unix

# 1. DEFAULT
all: test $(LIB) install_script
	@echo  [PKG]  Generating Distribution Artifacts v$(VERSION)...
	@$(MKDIR_INC)
	@$(CP_INC)
	@echo  [PKG]  Artifacts ready at $(OUT_DIR)

# OS-Specific Installation Scripts
install_script: $(INSTALL_SCRIPT)

install_unix: dirs
	@echo '#!/bin/bash' > $(OUT_DIR)/install.sh
	@echo 'cd "$$(dirname "$$0")" || exit' >> $(OUT_DIR)/install.sh
	@echo 'if [ "$$EUID" -ne 0 ]; then echo "[ERROR] Please run as root"; exit 1; fi' >> $(OUT_DIR)/install.sh
	@echo 'echo "[INSTALL] Installing Camelot v$(VERSION) to /usr/local..."' >> $(OUT_DIR)/install.sh
	@echo 'mkdir -p /usr/local/include/camelot' >> $(OUT_DIR)/install.sh
	@echo 'cp -r include/* /usr/local/include/' >> $(OUT_DIR)/install.sh
	@echo 'cp libcamelot.a /usr/local/lib/' >> $(OUT_DIR)/install.sh
	@echo 'echo "[DONE]    Camelot v$(VERSION) installed."' >> $(OUT_DIR)/install.sh
	@chmod +x $(OUT_DIR)/install.sh 2>$(DEV_NULL) || $(TRUE_CMD)

install_win: dirs
	@echo @echo off > $(OUT_DIR)/install.bat
	@echo cd /d "%%~dp0" >> $(OUT_DIR)/install.bat
	@echo echo [INSTALL] Installing Camelot v$(VERSION) to C:\Camelot... >> $(OUT_DIR)/install.bat
	@echo if not exist C:\Camelot\include mkdir C:\Camelot\include >> $(OUT_DIR)/install.bat
	@echo if not exist C:\Camelot\lib mkdir C:\Camelot\lib >> $(OUT_DIR)/install.bat
	@echo xcopy /E /I /Y include C:\Camelot\include ^>NUL >> $(OUT_DIR)/install.bat
	@echo copy /Y libcamelot.a C:\Camelot\lib\ ^>NUL >> $(OUT_DIR)/install.bat
	@echo :: Set Environment Variables (User/Machine Scope) >> $(OUT_DIR)/install.bat
	@echo setx C_INCLUDE_PATH "C:\Camelot\include" ^>NUL >> $(OUT_DIR)/install.bat
	@echo setx LDFLAGS "-LC:\Camelot\lib -lcamelot" ^>NUL >> $(OUT_DIR)/install.bat
	@echo echo [DONE] Camelot Setup Complete. >> $(OUT_DIR)/install.bat
	@echo pause >> $(OUT_DIR)/install.bat

# 2. INTERNAL Utility
dirs:
	@$(MKDIR_BUILD)

# 3. TEST SUITE
test: dirs
	@echo  [CC]   Compiling Test Suite...
	@$(CC) $(CFLAGS) $(TEST_SRCS) $(SRCS) -o $(TEST_TARGET)
	@echo  [EXEC] Running Tests...
	@./$(TEST_TARGET)
	@$(RM) test_stdin.tmp 2>$(DEV_NULL) || $(TRUE_CMD)

# 4. LIBRARY GENERATION
%.o: %.c
	@echo  [CC]   $<
	@$(CC) $(CFLAGS) -c $< -o $@

$(LIB): dirs $(OBJS)
	@echo  [AR]   Creating Static Library...
	@$(AR) rcs $(LIB) $(OBJS)
	@$(RM_OBJS) 2>$(DEV_NULL) || $(TRUE_CMD)

# 5. CLEANUP
clean:
	@echo  [RM]   Cleaning build artifacts...
	@$(RMDIR) $(OUT_DIR) 2>$(DEV_NULL) || $(TRUE_CMD)
	@$(RM_OBJS) 2>$(DEV_NULL) || $(TRUE_CMD)
	@$(RM) NUL test_stdin.tmp 2>$(DEV_NULL) || $(TRUE_CMD)

# 6. HELP
help:
	@echo --- Camelot Build System ---
	@echo Available commands:
	@echo   make       - Run tests and build installable packages for Windows and UNIX
	@echo   make test  - Run the test suite
	@echo   make clean - Remove all build and test artifacts
	@echo   make help  - Show this help message
	@echo.
	@echo Installation:
	@echo   After running 'make', execute the install script in the build output directory:
	@echo   - Windows: Run build\install.bat as Administrator (Right click -^> Run as Administrator)
	@echo   - UNIX: Run sudo ./build/install.sh
	@echo.
	@echo Compilation (After Install):
	@echo   Compile programs with the `-lcamelot` linkering flag!
	@echo   - UNIX: gcc main.c -lcamelot
	@echo   - Windows: gcc main.c -lcamelot
	@echo   * Note: Windows install sets up library paths automatically via LDFLAGS.
