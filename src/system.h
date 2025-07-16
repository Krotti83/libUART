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

#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

#include <UART.h>

#define DEV_NAME_LEN        256

#define DEV_FLAGS_OPENED    0x00000001
#define DEV_FLAGS_ERROR     0x80000000

struct _uart {
#ifdef __unix__
    int fd;
#ifdef LIBUART_THREADS
    pthread_t rx_thread;
    pthread_t tx_thread;
    pthread_mutex_t rx_mutex;
    pthread_mutex_t tx_mutex;
    int rx_thread_run;
    int tx_thread_run;
    pthread_mutex_t rx_lock;
    pthread_mutex_t tx_lock;
#endif
#elif _WIN32
    HANDLE h;
    COMMPROP prop;
#ifdef LIBUART_THREADS
    HANDLE rx_thread;
    HANDLE tx_thread;
    HANDLE rx_mutex;
    HANDLE tx_mutex;
    int rx_mutex_run;
    int tx_mutex_run;
    HANDLE rx_lock;
    HANDLE tx_lock;
#endif
#endif
    buffer_t rx_buffer;
    buffer_t tx_buffer;
    char dev[DEV_NAME_LEN];
    int baud;
    int data_bits;
    int stop_bits;
    int parity;
    int flow_ctrl;
    int error;
    char *err_msg;
    int err_msg_len;
    unsigned int flags;
};

extern int _uart_baud_valid(int value);
extern int _uart_init_baud(struct _uart *uart);
extern int _uart_init_databits(struct _uart *uart);
extern int _uart_init_parity(struct _uart *uart);
extern int _uart_init_stopbits(struct _uart *uart);
extern int _uart_init_flow(struct _uart *uart);
extern int _uart_init(void);
extern int _uart_open(struct _uart *uart);
extern void _uart_close(struct _uart *uart);
extern ssize_t _uart_send(struct _uart *uart, char *send_buf, size_t len);
extern ssize_t _uart_recv(struct _uart *uart, char *recv_buf, size_t len);
extern int _uart_flush(struct _uart *uart);
extern int _uart_set_pin(struct _uart *uart, enum e_pins pin, int state);
extern int _uart_get_pin(struct _uart *uart, enum e_pins pin, int *state);
extern int _uart_get_bytes(struct _uart *uart, int *bytes);

#endif
