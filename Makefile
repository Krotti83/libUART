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

BUILD_DIR		= build
BUILD_CONFIG		= $(BUILD_DIR)/config.mk

TARGET			= libUART

-include $(BUILD_CONFIG)

ifneq ($(CONFIG_CONFIGURED),yes)
$(error "$(TARGET) build system not configured, run './configure' in root directory")
endif

TARGET_STATIC		= $(TARGET).a
TARGET_DYNAMIC		= $(TARGET).so
TARGET_WINDLL		= $(TARGET).dll

# Generic shell commands
RM			= rm -rf
CP			= cp -Rf
MKDIR			= mkdir -p
MV			= mv -f
LN			= ln -sf
CD			= cd
INSTALL			= install

# Toolchain
ifneq ($(CROSS_COMPILE),)
AR			= $(CROSS_COMPILE)ar
CC			= $(CROSS_COMPILE)gcc
LD			= $(CROSS_COMPILE)gcc
else
include mk/compiler.mk
endif

PDFLATEX		= pdflatex

# pdflatex flags
PDFLATEXFLAGS		+= -interaction batchmode
PDFLATEXFLAGS		+= -output-directory $(BUILD_DIR)/doc

# Generic C compiler flags
CCFLAGS			+= -g
CCFLAGS			+= -Wall
CCFLAGS			+= -Wextra
CCFLAGS			+= -pedantic
CCFLAGS			+= -std=c99
CCFLAGS			+= -ffunction-sections
CCFLAGS			+= -fdata-sections
CCFLAGS			+= -fno-delete-null-pointer-checks

ifeq ($(CONFIG_BUILD_THREADS),yes)
CCFLAGS			+= -DLIBUART_THREADS
CCFLAGS			+= -D_DEFAULT_SOURCE
endif

# Dynamic C compiler flags
CCFLAGS_DYN		+= -fPIC

CCFLAGS_WINDLL		+= -DLIBUART_EXPORTS

# Generic GNU ld flags
LDFLAGS			+= -Wl,-n
LDFLAGS			+= -Wl,--build-id=none
LDFLAGS			+= -Wl,--gc-sections

ifeq ($(CONFIG_BUILD_COMPILER),gcc)
LDFLAGS			+= -Wl,--no-warn-rwx-segments
endif

# Dynamic GNU ld flags
LDFLAGS_DYN		+= -Wl,-soname,$(TARGET_DYNAMIC).0.2
LDFLAGS_DYN		+= -Wl,-shared
LDFLAGS_DYN		+= -Wl,-Bdynamic

LDFLAGS_WINDLL		+= -Wl,-shared
LDFLAGS_WINDLL		+= -Wl,--subsystem,windows
LDFLAGS_WINDLL		+= -Wl,--out-implib,$(BUILD_DIR)/$(TARGET).lib

# Defines
ifneq ($(CONFIG_GIT_VERSION),)
CCFLAGS			+= -DGIT_VERSION=$(CONFIG_GIT_VERSION)
endif

include ./src/files.mk

LIBUART_SCOBJ		+= $(LIBUART_SCSRC:.c=.o)
LIBUART_DCOBJ		+= $(LIBUART_DCSRC:.c=.o)
LIBUART_DROBJ		+= $(LIBUART_DRSRC:.rc=.o)

ifneq ($(CONFIG_BUILD_OS),win32)
ifneq ($(CONFIG_BUILD_DOC),yes)
ifeq ($(CONFIG_BUILD_STATIC),yes)
ifeq ($(CONFIG_BUILD_SHARED),yes)
all: copy_sources libuart_static libuart_dynamic link_dynamic
else
all: copy_sources libuart_static
endif
else
ifeq ($(CONFIG_BUILD_SHARED),yes)
all: copy_sources libuart_dynamic link_dynamic
else
all: copy_sources
endif
endif
else
ifeq ($(CONFIG_BUILD_STATIC),yes)
ifeq ($(CONFIG_BUILD_SHARED),yes)
all: copy_sources libuart_static libuart_dynamic link_dynamic build_doc
else
all: copy_sources libuart_static build_doc
endif
else
ifeq ($(CONFIG_BUILD_SHARED),yes)
all: copy_sources libuart_dynamic link_dynamic build_doc
else
all: copy_sources build_doc
endif
endif
endif
else
ifneq ($(CONFIG_BUILD_DOC),yes)
all: copy_sources libuart_windll
else
all: copy_sources libuart_windll build_doc
endif
endif

#
# libUART (static)
#

# Archive object files
ifeq ($(CONFIG_BUILD_STATIC),yes)
libuart_static: $(LIBUART_SCOBJ)
	@echo "   [AR]       $(BUILD_DIR)/$(TARGET_STATIC)"
	@$(AR) rcs $(BUILD_DIR)/$(TARGET_STATIC) \
	$(LIBUART_SCOBJ)

# Compile C sources
$(LIBUART_SCOBJ): %.o: %.c
	@echo "   [CC]       $@"
	@$(CC) -c $(CCFLAGS) -I $(BUILD_DIR) -o $@ $<
endif

#
# libUART (dynamic)
#

# Link object files
ifeq ($(CONFIG_BUILD_SHARED),yes)
ifneq ($(CONFIG_BUILD_OS),win32)
ifneq ($(CONFIG_BUILD_THREADS),yes)
libuart_dynamic: $(LIBUART_DCOBJ)
	@echo "   [LD]       $(BUILD_DIR)/$(TARGET_DYNAMIC).0.2"
	@$(CC) -shared $(CCFLAGS) $(CCFLAGS_DYN) \
	$(LDFLAGS) $(LDFLAGS_DYN) \
	-o $(BUILD_DIR)/$(TARGET_DYNAMIC).0.2 \
	$(LIBUART_DCOBJ)
else
libuart_dynamic: $(LIBUART_DCOBJ)
	@echo "   [LD]       $(BUILD_DIR)/$(TARGET_DYNAMIC).0.2"
	@$(CC) -shared $(CCFLAGS) $(CCFLAGS_DYN) \
	$(LDFLAGS) $(LDFLAGS_DYN) \
	-o $(BUILD_DIR)/$(TARGET_DYNAMIC).0.2 \
	$(LIBUART_DCOBJ) -lpthread
endif

# Compile C sources
$(LIBUART_DCOBJ): %.o: %.c
	@echo "   [CC]       $@"
	@$(CC) -c $(CCFLAGS) $(CCFLAGS_DYN) -I $(BUILD_DIR) -o $@ $<

else
# Link Windows DLL
libuart_windll: $(LIBUART_DCOBJ) $(LIBUART_DROBJ)
	@echo "   [LD]       $(BUILD_DIR)/$(TARGET_WINDLL)"
	@$(CC) -shared $(CCFLAGS) \
	$(LDFLAGS) $(LDFLAGS_WINDLL) \
	-o $(BUILD_DIR)/$(TARGET_WINDLL) $(LIBUART_DCOBJ) $(LIBUART_DROBJ)

# Compile C sources
$(LIBUART_DCOBJ): %.o: %.c
	@echo "   [CC]       $@"
	@$(CC) -c $(CCFLAGS) $(CCFLAGS_WINDLL) -I $(BUILD_DIR) -o $@ $<


# Compile Windows resources
$(LIBUART_DROBJ): %.o: %.rc
	@echo "   [WINDRES]  $@"
	@$(WINDRES) -o $@ -i $<
endif
endif

# Install
install: install_libUART

# Install libUART
.PHONY: install_libUART
install_libUART:
ifneq ($(CONFIG_BUILD_OS),win32)
	@echo "   [INSTALL]  $(INSTALL_INCDIR)/UART.h"
	@$(INSTALL) -d $(INSTALL_INCDIR)
	@$(INSTALL) -m 644 -D $(BUILD_DIR)/UART.h $(INSTALL_INCDIR)
	@$(INSTALL) -d $(INSTALL_LIBDIR)
ifeq ($(CONFIG_BUILD_SHARED),yes)
	@$(INSTALL) -m 755 -D $(BUILD_DIR)/*.so.0.2 $(INSTALL_LIBDIR)
	@$(INSTALL) -m 755 -D $(BUILD_DIR)/*.so.0 $(INSTALL_LIBDIR)
	@$(INSTALL) -m 755 -D $(BUILD_DIR)/*.so $(INSTALL_LIBDIR)
endif
ifeq ($(CONFIG_BUILD_STATIC),yes)
	@$(INSTALL) -m 644 -D $(BUILD_DIR)/*.a $(INSTALL_LIBDIR)
endif
ifeq ($(CONFIG_BUILD_DOC),yes)
	@$(INSTALL) -d $(INSTALL_DOCDIR)
	@$(INSTALL) -m 644 -D $(BUILD_DIR)/libUART.pdf $(INSTALL_DOCDIR)
endif
endif

# Build documentation
ifeq ($(CONFIG_BUILD_DOC),yes)
.PHONY: build_doc
build_doc:
	@echo "   [PDFLATEX] $(BUILD_DIR)/$(TARGET).pdf"
	@$(PDFLATEX) $(PDFLATEXFLAGS) $(BUILD_DIR)/doc/$(TARGET).tex 1>/dev/null
	@$(MV) $(BUILD_DIR)/doc/$(TARGET).pdf $(BUILD_DIR)/$(TARGET).pdf
	@$(PDFLATEX) $(PDFLATEXFLAGS) $(BUILD_DIR)/doc/$(TARGET).tex 1>/dev/null
	@$(MV) $(BUILD_DIR)/doc/$(TARGET).pdf $(BUILD_DIR)/$(TARGET).pdf
endif

# Clean (remove build directory)
.PHONY: clean
clean:
	@echo "   [CLEAN]    $(BUILD_DIR)"
	@$(RM) $(BUILD_DIR)

# Copy sources in build directory
.PHONY: copy_sources
copy_sources:
ifneq ($(CONFIG_BUILD_OS),win32)
ifeq ($(CONFIG_BUILD_SHARED),yes)
	@$(MKDIR) $(BUILD_DIR)/dynamic
	@$(CP) $(LIBUART_DIR)/*.c $(BUILD_DIR)/dynamic
	@$(CP) $(LIBUART_DIR)/*.h $(BUILD_DIR)/dynamic
	@$(CP) $(LIBUART_DIR)/*.mk $(BUILD_DIR)/dynamic
endif
ifeq ($(CONFIG_BUILD_STATIC),yes)
	@$(MKDIR) $(BUILD_DIR)/static
	@$(CP) $(LIBUART_DIR)/*.c $(BUILD_DIR)/static
	@$(CP) $(LIBUART_DIR)/*.h $(BUILD_DIR)/static
	@$(CP) $(LIBUART_DIR)/*.mk $(BUILD_DIR)/static
endif
else
	@$(MKDIR) $(BUILD_DIR)/dynamic
	@$(CP) $(LIBUART_DIR)/*.c $(BUILD_DIR)/dynamic
	@$(CP) $(LIBUART_DIR)/*.h $(BUILD_DIR)/dynamic
	@$(CP) $(LIBUART_DIR)/*.mk $(BUILD_DIR)/dynamic
	@$(CP) $(LIBUART_DIR)/*.rc $(BUILD_DIR)/dynamic
endif
	@$(CP) $(LIBUART_DIR)/include/*.h $(BUILD_DIR)
ifeq ($(CONFIG_BUILD_DOC),yes)
	@$(MKDIR) $(BUILD_DIR)/doc
	@$(CP) doc/$(TARGET).tex $(BUILD_DIR)/doc
endif

# Create symbolic link for dynamic (shared) library
ifeq ($(CONFIG_BUILD_SHARED),yes)
.PHONY: link_dynamic
link_dynamic:
	@echo "   [LN]       $(BUILD_DIR)/$(TARGET_DYNAMIC).0 -> $(BUILD_DIR)/$(TARGET_DYNAMIC).0.2"
	@$(LN) $(TARGET_DYNAMIC).0.2 $(BUILD_DIR)/$(TARGET_DYNAMIC).0
	@echo "   [LN]       $(BUILD_DIR)/$(TARGET_DYNAMIC) -> $(BUILD_DIR)/$(TARGET_DYNAMIC).0"
	@$(LN) $(TARGET_DYNAMIC).0 $(BUILD_DIR)/$(TARGET_DYNAMIC)
endif
