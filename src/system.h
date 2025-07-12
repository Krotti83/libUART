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

#ifndef _LIBUART_SYSTEM_H
#define _LIBUART_SYSTEM_H

#define DEV_NAME_LEN        256

struct _uart {
    int fd;
    char dev[DEV_NAME_LEN];
    int baud;
    int data_bits;
    int stop_bits;
    int parity;
    int flow_ctrl;
    int errno;
    char *err_msg;
    int err_msg_len;
};

extern int uart_baud_valid(int value);
extern int uart_init_baud(struct _uart *uart);
extern int uart_init_databits(struct _uart *uart);
extern int uart_init_parity(struct _uart *uart);
extern int uart_init_stopbits(struct _uart *uart);
extern int uart_init_flow(struct _uart *uart);
extern int uart_init(struct _uart *uart);
extern int uart_open(struct _uart *uart);
extern void uart_close(struct _uart *uart);
extern int uart_send(struct _uart *uart, char *send_buf, int len);
extern int uart_recv(struct _uart *uart, char *recv_buf, int len);
extern int uart_flush(struct _uart *uart);
extern int uart_set_pin(struct _uart *uart, int pin, int state);
extern int uart_get_pin(struct _uart *uart, int pin, int *state);
extern int uart_get_bytes(struct _uart *uart, int *bytes);

#endif
