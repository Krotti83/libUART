#
# libUART
#
# Easy to use library for accessing the UART
#
# Copyright (c) 2025 Johannes Krottmayer <krotti83@proton.me>
#
# Permission to use, copy, modify, and/or distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
# ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
# ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
# OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
#

BUILD_DIR		= ./build
BUILD_CONFIG		= $(TARGET)/config.mk

TARGET			= libUART

TARGET_STATIC		= $(TARGET).a
TARGET_DYNAMIC		= $(TARGET).so

# Generic shell commands
RM			= rm -rf
CP			= cp -Rf
MKDIR			= mkdir -p
LN			= ln -sf
CD			= cd

# Toolchain
AR			= $(CROSS_COMPILE)ar
CC			= $(CROSS_COMPILE)gcc
LD			= $(CROSS_COMPILE)gcc

# Generic C compiler flags
CCFLAGS			+= -g
CCFLAGS			+= -Wall
CCFLAGS			+= -Wextra
CCFLAGS			+= -pedantic
CCFLAGS			+= -std=c99
CCFLAGS			+= -ffunction-sections
CCFLAGS			+= -fdata-sections
CCFLAGS			+= -fno-delete-null-pointer-checks

# Dynamic C compiler flags
CCFLAGS_DYN		+= -fPIC

# Generic GNU ld flags
LDFLAGS			+= -Wl,-n
LDFLAGS			+= -Wl,--build-id=none
LDFLAGS			+= -Wl,--gc-sections
LDFLAGS			+= -Wl,--no-warn-rwx-segments

# Dynamic GNU ld flags
LDFLAGS_DYN		+= -Wl,-soname,$(TARGET_DYNAMIC).0.1
LDFLAGS_DYN		+= -Wl,-shared
LDFLAGS_DYN		+= -Wl,-Bdynamic

-include $(BUILD_CONFIG)

include ./src/files.mk

LIBUART_SCOBJ		+= $(LIBUART_SCSRC:.c=.o)
LIBUART_DCOBJ		+= $(LIBUART_DCSRC:.c=.o)

all: copy_sources libuart_static libuart_dynamic link_dynamic

#
# libUART (static)
#

# Archive object files
libuart_static: $(LIBUART_SCOBJ)
	@echo "   [AR]   $(BUILD_DIR)/$(TARGET_STATIC)"
	@$(AR) rcs $(BUILD_DIR)/$(TARGET_STATIC) \
	$(LIBUART_SCOBJ)

# Compile C sources
$(LIBUART_SCOBJ): %.o: %.c
	@echo "   [CC]   $@"
	@$(CC) -c $(CCFLAGS) -I $(BUILD_DIR) -o $@ $<

#
# libUART (dynamic)
#

# Link object files
libuart_dynamic: $(LIBUART_DCOBJ)
	@echo "   [LD]   $(BUILD_DIR)/$(TARGET_DYNAMIC).0.1"
	@$(CC) -shared $(CCFLAGS) $(CCFLAGS_DYN) \
	$(LDFLAGS) $(LDFLAGS_DYN) \
	-o $(BUILD_DIR)/$(TARGET_DYNAMIC).0.1 \
	$(LIBUART_DCOBJ)

# Compile C sources
$(LIBUART_DCOBJ): %.o: %.c
	@echo "   [CC]   $@"
	@$(CC) -c $(CCFLAGS) $(CCFLAGS_DYN) -I $(BUILD_DIR) -o $@ $<

.PHONY: clean
clean:
	@echo "   [CLEAN]"
	@$(RM) $(BUILD_DIR)

.PHONY: check_config
check_config:
	@echo "   [CHECK]"
	@$(RM) $(BUILD_DIR)
	@$(RM) $(TARGET_STATIC)

.PHONY: copy_sources
copy_sources:
	@$(MKDIR) $(BUILD_DIR)/static
	@$(MKDIR) $(BUILD_DIR)/dynamic
	@$(CP) $(LIBUART_DIR)/*.c $(BUILD_DIR)/static
	@$(CP) $(LIBUART_DIR)/*.h $(BUILD_DIR)/static
	@$(CP) $(LIBUART_DIR)/*.mk $(BUILD_DIR)/static
	@$(CP) $(LIBUART_DIR)/*.c $(BUILD_DIR)/dynamic
	@$(CP) $(LIBUART_DIR)/*.h $(BUILD_DIR)/dynamic
	@$(CP) $(LIBUART_DIR)/include/*.h $(BUILD_DIR)
	@$(CP) $(LIBUART_DIR)/*.mk $(BUILD_DIR)/dynamic

.PHONY: link_dynamic
link_dynamic:
	@echo "   [LN]   $(BUILD_DIR)/$(TARGET_DYNAMIC).0 -> $(BUILD_DIR)/$(TARGET_DYNAMIC).0.1"
	@$(LN) $(TARGET_DYNAMIC).0.1 $(BUILD_DIR)/$(TARGET_DYNAMIC).0
	@echo "   [LN]   $(BUILD_DIR)/$(TARGET_DYNAMIC) -> $(BUILD_DIR)/$(TARGET_DYNAMIC).0"
	@$(LN) $(TARGET_DYNAMIC).0 $(BUILD_DIR)/$(TARGET_DYNAMIC)
