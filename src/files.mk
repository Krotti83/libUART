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
LIBUART_SCSRC				+= $(BUILD_DIR)/static/posix_uart.c
LIBUART_SCSRC				+= $(BUILD_DIR)/static/uart.c
LIBUART_SCSRC				+= $(BUILD_DIR)/static/util.c

ifeq ($(CONFIG_BUILD_THREADS),yes)
LIBUART_SCSRC				+= $(BUILD_DIR)/static/buffer.c
LIBUART_SCSRC				+= $(BUILD_DIR)/static/posix_thread.c
endif

LIBUART_DCSRC				+= $(BUILD_DIR)/dynamic/posix_error.c
LIBUART_DCSRC				+= $(BUILD_DIR)/dynamic/posix_uart.c
LIBUART_DCSRC				+= $(BUILD_DIR)/dynamic/uart.c
LIBUART_DCSRC				+= $(BUILD_DIR)/dynamic/util.c

ifeq ($(CONFIG_BUILD_THREADS),yes)
LIBUART_DCSRC				+= $(BUILD_DIR)/dynamic/buffer.c
LIBUART_DCSRC				+= $(BUILD_DIR)/dynamic/posix_thread.c
endif

else
LIBUART_DCSRC				+= $(BUILD_DIR)/dynamic/win32_error.c
LIBUART_DCSRC				+= $(BUILD_DIR)/dynamic/win32_uart.c
LIBUART_DCSRC				+= $(BUILD_DIR)/dynamic/uart.c
LIBUART_DCSRC				+= $(BUILD_DIR)/dynamic/util.c

ifeq ($(CONFIG_BUILD_THREADS),yes)
LIBUART_DCSRC				+= $(BUILD_DIR)/dynamic/buffer.c
LIBUART_DCSRC				+= $(BUILD_DIR)/dynamic/win32_thread.c
endif

LIBUART_DRSRC				+= $(BUILD_DIR)/dynamic/win32_resource.rc
endif
