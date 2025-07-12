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

TARGET			= libUART

TARGET_STATIC		= $(TARGET).a

# Generic shell commands
RM			= rm -rf
CP			= cp -Rf

# Toolchain
AR			= $(CROSS_COMPILE)ar
CC			= $(CROSS_COMPILE)gcc
LD			= $(CROSS_COMPILE)gcc

# C compiler flags
CCFLAGS			+= -g
CCFLAGS			+= -Wall
CCFLAGS			+= -Wextra
CCFLAGS			+= -pedantic
CCFLAGS			+= -std=c99
CCFLAGS			+= -ffunction-sections
CCFLAGS			+= -fdata-sections
CCFLAGS			+= -fno-delete-null-pointer-checks

# GNU ld flags
LDFLAGS			+= -Wl,-n
LDFLAGS			+= -Wl,--build-id=none
LDFLAGS			+= -Wl,--gc-sections

include ./src/files.mk

LIBUART_COBJ		+= $(LIBUART_CSRC:.c=.o)

all: libuart_static

#
# libUART (static)
#

# Link object files
libuart_static: $(LIBUART_COBJ)
	@echo "   [AR]   $(TARGET_STATIC)"
	@$(AR) rcs $(TARGET_STATIC) \
	$(LIBUART_COBJ)

# Compile C sources
$(LIBUART_COBJ): %.o: %.c
	@echo "   [CC]   $@"
	@$(CC) -c $(CCFLAGS) -o $@ $<

.PHONY: clean
clean:
	@echo "   [CLEAN]"
	@$(RM) $(LIBUART_COBJ)
	@$(RM) $(TARGET_STATIC)
