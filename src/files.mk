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

LIBUART_DIR				= ./src

ifneq ($(CONFIG_BUILD_OS),win32)
LIBUART_SCSRC				+= $(BUILD_DIR)/static/posix_error.c
LIBUART_SCSRC				+= $(BUILD_DIR)/static/posix_system.c
LIBUART_SCSRC				+= $(BUILD_DIR)/static/uart.c
LIBUART_SCSRC				+= $(BUILD_DIR)/static/util.c

LIBUART_DCSRC				+= $(BUILD_DIR)/dynamic/posix_error.c
LIBUART_DCSRC				+= $(BUILD_DIR)/dynamic/posix_system.c
LIBUART_DCSRC				+= $(BUILD_DIR)/dynamic/uart.c
LIBUART_DCSRC				+= $(BUILD_DIR)/dynamic/util.c
else
LIBUART_DCSRC				+= $(BUILD_DIR)/dynamic/win32_error.c
LIBUART_DCSRC				+= $(BUILD_DIR)/dynamic/win32_system.c
LIBUART_DCSRC				+= $(BUILD_DIR)/dynamic/uart.c
LIBUART_DCSRC				+= $(BUILD_DIR)/dynamic/util.c

LIBUART_DRSRC				+= $(BUILD_DIR)/dynamic/win32_resource.rc
endif
