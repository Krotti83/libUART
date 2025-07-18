/**
 *
 * libUART
 *
 * Easy to use library for accessing the UART
 *
 * Copyright (c) 2025 Johannes Krottmayer <krotti83@proton.me>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

#ifndef _LIBUART_INTERNAL_THREAD_H
#define _LIBUART_INTERNAL_THREAD_H

#include "_uart.h"

extern int _uart_thread_init(struct _uart_ctx *ctx, struct _uart *uart);
extern int _uart_thread_start(struct _uart_ctx *ctx, struct _uart *uart);
extern int _uart_thread_stop(struct _uart_ctx *ctx, struct _uart *uart);
extern void _uart_thread_lock_rx(struct _uart_ctx *ctx, struct _uart *uart);
extern void _uart_thread_lock_tx(struct _uart_ctx *ctx, struct _uart *uart);
extern void _uart_thread_unlock_rx(struct _uart_ctx *ctx, struct _uart *uart);
extern void _uart_thread_unlock_tx(struct _uart_ctx *ctx, struct _uart *uart);

#endif
