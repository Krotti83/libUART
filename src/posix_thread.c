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

#include <pthread.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "_uart.h"
#include "_buffer.h"

#include <UART.h>

struct _thread_args {
    struct _uart_ctx *ctx;
    struct _uart *uart;
};

#define THREAD_SLEEP_1MS        1000
#define THREAD_BUFFER_SIZE      4096

int _uart_thread_init(struct _uart_ctx *ctx, struct _uart *uart)
{
    int ret;

    if (!ctx) {
        return UART_ECTX;
    }

    if (!uart) {
        _uart_error(ctx, NULL, UART_EHANDLE, NULL, "NULL");

        return UART_EHANDLE;
    }

    ret = pthread_mutex_init(&uart->rx_mutex, NULL);

    if (ret != 0) {
        _uart_error(ctx, uart, UART_ESYSAPI, "pthread_mutex_init", NULL);

        return UART_ESYSAPI;
    }

    ret = pthread_mutex_init(&uart->tx_mutex, NULL);

    if (ret != 0) {
        _uart_error(ctx, uart, UART_ESYSAPI, "pthread_mutex_init", NULL);

        return UART_ESYSAPI;
    }

    ret = pthread_mutex_init(&uart->rx_lock, NULL);

    if (ret != 0) {
        _uart_error(ctx, uart, UART_ESYSAPI, "pthread_mutex_init", NULL);

        return UART_ESYSAPI;
    }

    ret = pthread_mutex_init(&uart->tx_lock, NULL);

    if (ret != 0) {
        _uart_error(ctx, uart, UART_ESYSAPI, "pthread_mutex_init", NULL);

        return UART_ESYSAPI;
    }

    return UART_ESUCCESS;
}

void *worker_thread_rx(void *p)
{
    int run = 1;
    struct _thread_args *args = (struct _thread_args *) p;
    ssize_t len;
    ssize_t ret;
    unsigned char buf[THREAD_BUFFER_SIZE];
    int bytes;
    int ret_ioctl;

    while (run) {
        pthread_mutex_lock(&args->uart->rx_lock);

        ret_ioctl = ioctl(args->uart->fd, FIONREAD, &bytes);

        if (ret_ioctl == -1) {
            _uart_error(args->ctx, args->uart, UART_ESYSAPI, "ioctl", NULL);
            pthread_mutex_unlock(&args->uart->rx_lock);
            pthread_mutex_lock(&args->uart->rx_mutex);
            args->uart->rx_thread_run = 0;
            pthread_mutex_unlock(&args->uart->rx_mutex);

            return NULL;
        }

        len = buffer_get_free(args->uart->rx_buffer);

        if (len >= bytes) {
            ret = read(args->uart->fd, buf, bytes);

            if (ret == -1) {
                _uart_error(args->ctx, args->uart, UART_ESYSAPI, "read", NULL);
                pthread_mutex_unlock(&args->uart->rx_lock);
                pthread_mutex_lock(&args->uart->rx_mutex);
                args->uart->rx_thread_run = 0;
                pthread_mutex_unlock(&args->uart->rx_mutex);

                return NULL;
            }

            buffer_wr(args->uart->rx_buffer, buf, bytes);
        }

        pthread_mutex_unlock(&args->uart->rx_lock);
        pthread_mutex_lock(&args->uart->rx_mutex);

        if (!args->uart->rx_thread_run) {
            run = 0;
        }

        pthread_mutex_unlock(&args->uart->rx_mutex);
        usleep(THREAD_SLEEP_1MS);
    }

    return NULL;
}

void *worker_thread_tx(void *p)
{
    int run = 1;
    struct _thread_args *args = (struct _thread_args *) p;
    ssize_t ret;
    unsigned char buf[THREAD_BUFFER_SIZE];
    ssize_t len;


    while (run) {
        pthread_mutex_lock(&args->uart->tx_lock);
        len = buffer_get_num(args->uart->tx_buffer);

        if (len >= THREAD_BUFFER_SIZE) {
            buffer_rd(args->uart->tx_buffer, buf, THREAD_BUFFER_SIZE);
            ret = write(args->uart->fd, buf, THREAD_BUFFER_SIZE);

            if (ret == -1) {
                _uart_error(args->ctx, args->uart, UART_ESYSAPI, "write", NULL);
                pthread_mutex_unlock(&args->uart->tx_lock);
                pthread_mutex_lock(&args->uart->tx_mutex);
                args->uart->tx_thread_run = 0;
                pthread_mutex_unlock(&args->uart->tx_mutex);

                return NULL;
            }

            if (ret != (ssize_t) len) {
                _uart_error(args->ctx, args->uart, UART_ESYSAPI, "write", "could not send all data");
                pthread_mutex_unlock(&args->uart->tx_lock);

                return NULL;
            }
        } else if (len > 1) {
            buffer_rd(args->uart->tx_buffer, buf, len);
            ret = write(args->uart->fd, buf, len);

            if (ret == -1) {
                _uart_error(args->ctx, args->uart, UART_ESYSAPI, "write", NULL);
                pthread_mutex_unlock(&args->uart->tx_lock);
                pthread_mutex_lock(&args->uart->tx_mutex);
                args->uart->tx_thread_run = 0;
                pthread_mutex_unlock(&args->uart->tx_mutex);

                return NULL;
            }

            if (ret != (ssize_t) len) {
                _uart_error(args->ctx, args->uart, UART_ESYSAPI, "write", "could not send all data");
                pthread_mutex_unlock(&args->uart->tx_lock);
                pthread_mutex_lock(&args->uart->tx_mutex);
                args->uart->tx_thread_run = 0;
                pthread_mutex_unlock(&args->uart->tx_mutex);

                return NULL;
            }
        }

        pthread_mutex_unlock(&args->uart->tx_lock);
        pthread_mutex_lock(&args->uart->tx_mutex);

        if (!args->uart->tx_thread_run) {
            run = 0;
        }

        pthread_mutex_unlock(&args->uart->tx_mutex);
        usleep(THREAD_SLEEP_1MS);
    }

    return NULL;
}

int _uart_thread_start(struct _uart_ctx *ctx, struct _uart *uart)
{
    int ret;
    struct _thread_args args;

    if (!ctx) {
        return UART_ECTX;
    }

    if (!uart) {
        _uart_error(ctx, NULL, UART_EHANDLE, NULL, "NULL");

        return UART_EHANDLE;
    }

    args.ctx = ctx;
    args.uart = uart;
    uart->rx_thread_run = 1;
    uart->tx_thread_run = 1;
    ret = pthread_create(&uart->rx_thread, NULL, worker_thread_rx, (void *) &args);

    if (ret != 0) {
        _uart_error(ctx, uart, UART_ESYSAPI, "pthread_create", NULL);

        return UART_ESYSAPI;
    }

    ret = pthread_create(&uart->tx_thread, NULL, worker_thread_tx, (void *) &args);

    if (ret != 0) {
        _uart_error(ctx, uart, UART_ESYSAPI, "pthread_create", NULL);

        return UART_ESYSAPI;
    }

    return UART_ESUCCESS;
}

int _uart_thread_stop(struct _uart_ctx *ctx, struct _uart *uart)
{
    int ret;

    if (!ctx) {
        return UART_ECTX;
    }

    if (!uart) {
        _uart_error(ctx, NULL, UART_EHANDLE, NULL, "NULL");

        return UART_EHANDLE;
    }

    pthread_mutex_lock(&uart->rx_mutex);
    uart->rx_thread_run = 0;
    pthread_mutex_unlock(&uart->rx_mutex);

    pthread_mutex_lock(&uart->tx_mutex);
    uart->tx_thread_run = 0;
    pthread_mutex_unlock(&uart->tx_mutex);

    pthread_join(uart->rx_thread, NULL);
    pthread_join(uart->tx_thread, NULL);

    ret = pthread_mutex_destroy(&uart->rx_mutex);

    if (ret != 0) {
        _uart_error(ctx, uart, UART_ESYSAPI, "pthread_mutex_destroy", NULL);

        return UART_ESYSAPI;
    }

    ret = pthread_mutex_destroy(&uart->tx_mutex);

    if (ret != 0) {
        _uart_error(ctx, uart, UART_ESYSAPI, "pthread_mutex_destroy", NULL);

        return UART_ESYSAPI;
    }

    ret = pthread_mutex_destroy(&uart->rx_lock);

    if (ret != 0) {
        _uart_error(ctx, uart, UART_ESYSAPI, "pthread_mutex_destroy", NULL);

        return UART_ESYSAPI;
    }

    ret = pthread_mutex_destroy(&uart->tx_lock);

    if (ret != 0) {
        _uart_error(ctx, uart, UART_ESYSAPI, "pthread_mutex_destroy", NULL);

        return UART_ESYSAPI;
    }

    return UART_ESUCCESS;
}

int _uart_thread_lock_rx(struct _uart_ctx *ctx, struct _uart *uart)
{
    if (!ctx) {
        return UART_ECTX;
    }

    if (!uart) {
        _uart_error(ctx, NULL, UART_EHANDLE, NULL, "NULL");

        return UART_EHANDLE;
    }

    pthread_mutex_lock(&uart->rx_lock);

    return UART_ESUCCESS;
}

int _uart_thread_lock_tx(struct _uart_ctx *ctx, struct _uart *uart)
{
    if (!ctx) {
        return UART_ECTX;
    }

    if (!uart) {
        _uart_error(ctx, NULL, UART_EHANDLE, NULL, "NULL");

        return UART_EHANDLE;
    }

    pthread_mutex_lock(&uart->tx_lock);

    return UART_ESUCCESS;
}

int _uart_thread_unlock_rx(struct _uart_ctx *ctx, struct _uart *uart)
{
    if (!ctx) {
        return UART_ECTX;
    }

    if (!uart) {
        _uart_error(ctx, NULL, UART_EHANDLE, NULL, "NULL");

        return UART_EHANDLE;
    }

    pthread_mutex_unlock(&uart->rx_lock);

    return UART_ESUCCESS;
}

int _uart_thread_unlock_tx(struct _uart_ctx *ctx, struct _uart *uart)
{
    if (!ctx) {
        return UART_ECTX;
    }

    if (!uart) {
        _uart_error(ctx, NULL, UART_EHANDLE, NULL, "NULL");

        return UART_EHANDLE;
    }

    pthread_mutex_unlock(&uart->tx_lock);

    return UART_ESUCCESS;
}
