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

#ifndef _LIBUART_INTERNAL_UART_H
#define _LIBUART_INTERNAL_UART_H

#ifdef _WIN32
#include <windows.h>
#else
#ifdef LIBUART_THREADS
#include <pthread.h>
#endif
#endif

#ifdef LIBUART_THREADS
#include "_buffer.h"

#define UART_BUFFERSIZE         1048576
#endif

#include <UART.h>

#define UART_NAMEMAX            512
#define UART_ERRORMAX           512

#define UART_FOPENED            0x00000001
#define UART_FERROR             0x80000000

struct _uart {
    char dev[UART_NAMEMAX];
#ifdef __unix__
    int fd;
#elif _WIN32
    HANDLE h;
    COMMPROP prop;
#endif
    enum e_baud baud;
    enum e_data data_bits;
    enum e_stop stop_bits;
    enum e_parity parity;
    enum e_flow flow_ctrl;
    int error;
    char *errormsg;
    unsigned int flags;
#ifdef LIBUART_THREADS
#ifdef __unix__
    pthread_t rx_thread;
    pthread_t tx_thread;
    pthread_mutex_t rx_mutex;
    pthread_mutex_t tx_mutex;
    int rx_thread_run;
    int tx_thread_run;
    pthread_mutex_t rx_lock;
    pthread_mutex_t tx_lock;
#elif _WIN32
    HANDLE rx_thread;
    HANDLE tx_thread;
    HANDLE rx_mutex;
    HANDLE tx_mutex;
    int rx_thread_run;
    int tx_thread_run;
    HANDLE rx_lock;
    HANDLE tx_lock;
#endif
    buffer_t *rx_buffer;
    buffer_t *tx_buffer;
#endif
};

#define UART_DEVMAX             512

#define UART_CTXFOKAY           0x00000001
#define UART_CTXFERROR          0x80000000

struct _uart_ctx {
    int init_done;
    int error;
    char *errormsg;
    int uarts_count;
    struct _uart *uarts[UART_DEVMAX];
    unsigned int flags;
};

extern int _uart_get_device_list(struct _uart_ctx *ctx);

extern int _uart_baud_valid(int value);

extern int _uart_init_baud(struct _uart_ctx *ctx,
                           struct _uart *uart);

extern int _uart_init_databits(struct _uart_ctx *ctx,
                               struct _uart *uart);

extern int _uart_init_parity(struct _uart_ctx *ctx,
                             struct _uart *uart);

extern int _uart_init_stopbits(struct _uart_ctx *ctx,
                               struct _uart *uart);

extern int _uart_init_flow(struct _uart_ctx *ctx,
                           struct _uart *uart);

extern int _uart_init(struct _uart_ctx *ctx);

extern int _uart_open(struct _uart_ctx *ctx,
                      struct _uart *uart);

extern int _uart_close(struct _uart_ctx *ctx,
                       struct _uart *uart);

#ifndef LIBUART_THREADS
extern ssize_t _uart_send(struct _uart_ctx *ctx,
                          struct _uart *uart,
                          void *send_buf,
                          size_t len);

extern ssize_t _uart_recv(struct _uart_ctx *ctx,
                          struct _uart *uart,
                          void *recv_buf,
                          size_t len);
#endif

extern int _uart_flush(struct _uart_ctx *ctx,
                       struct _uart *uart);

extern int _uart_set_pin(struct _uart_ctx *ctx,
                         struct _uart *uart,
                         enum e_pins pin,
                         int state);

extern int _uart_get_pin(struct _uart_ctx *ctx,
                         struct _uart *uart,
                         enum e_pins pin,
                         int *state);

extern int _uart_get_bytes(struct _uart_ctx *ctx,
                           struct _uart *uart,
                           int *bytes);

extern void _uart_error(struct _uart_ctx *ctx,
                        struct _uart *uart,
                        int error,
                        const char *error_func,
                        const char *error_msg);

extern void _uart_perror(struct _uart_ctx *ctx,
                         struct _uart *uart);

#endif
