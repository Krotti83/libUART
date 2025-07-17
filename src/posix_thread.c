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

#include "system.h"
#include "buffer.h"

#include <UART.h>

#define THREAD_SLEEP_1MS        1000
#define THREAD_BUFFER_SIZE      4096

int _uart_thread_init(uart_t *uart)
{
    int ret;

    ret = pthread_mutex_init(&uart->rx_mutex, NULL);

    if (ret != 0) {
        return UART_ESYSTEM;
    }

    ret = pthread_mutex_init(&uart->tx_mutex, NULL);

    if (ret != 0) {
        return UART_ESYSTEM;
    }

    ret = pthread_mutex_init(&uart->rx_lock, NULL);

    if (ret != 0) {
        return UART_ESYSTEM;
    }

    ret = pthread_mutex_init(&uart->tx_lock, NULL);

    if (ret != 0) {
        return UART_ESYSTEM;
    }

    return UART_ESUCCESS;
}

void *worker_thread_rx(void *p)
{
    int run = 1;
    uart_t *uart = (uart_t *) p;
    ssize_t len;
    ssize_t ret;
    unsigned char buf[THREAD_BUFFER_SIZE];

    while (run) {
        pthread_mutex_lock(&uart->rx_lock);

        len = buffer_get_free(uart->rx_buffer);

        if (len > 1) {
            ret = read(uart->fd, buf, len);
            buffer_wr(uart->rx_buffer, buf, ret);
        }

        pthread_mutex_unlock(&uart->rx_lock);
        pthread_mutex_lock(&uart->rx_mutex);

        if (!uart->rx_thread_run) {
            run = 0;
            break;
        }

        pthread_mutex_unlock(&uart->rx_mutex);
        usleep(THREAD_SLEEP_1MS);
    }

    return UART_ESUCCESS;
}

void *worker_thread_tx(void *p)
{
    int run = 1;
    uart_t *uart = (uart_t *) p;
    ssize_t ret;
    unsigned char buf[THREAD_BUFFER_SIZE];
    ssize_t len;


    while (run) {
        pthread_mutex_lock(&uart->tx_lock);
        len = buffer_get_num(uart->tx_buffer);

        if (len >= THREAD_BUFFER_SIZE) {
            buffer_rd(uart->tx_buffer, buf, THREAD_BUFFER_SIZE);
            ret = write(uart->fd, buf, THREAD_BUFFER_SIZE);
        } else if (len > 1) {
            buffer_rd(uart->tx_buffer, buf, len);
            ret = write(uart->fd, buf, len);
        }

        pthread_mutex_unlock(&uart->tx_lock);
        pthread_mutex_lock(&uart->tx_mutex);

        if (!uart->tx_thread_run) {
            run = 0;
            break;
        }

        pthread_mutex_unlock(&uart->tx_mutex);
        usleep(THREAD_SLEEP_1MS);
    }

    return NULL;
}

int _uart_thread_start(uart_t *uart)
{
    int ret;

    uart->rx_thread_run = 1;
    uart->tx_thread_run = 1;
    ret = pthread_create(&uart->rx_thread, NULL, worker_thread_rx, (void *) uart);

    if (ret != 0) {
        return UART_ESYSTEM;
    }

    ret = pthread_create(&uart->tx_thread, NULL, worker_thread_tx, (void *) uart);

    if (ret != 0) {
        return UART_ESYSTEM;
    }

    return UART_ESUCCESS;
}

int _uart_thread_stop(uart_t *uart)
{
    int ret;

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
        return UART_ESYSTEM;
    }

    ret = pthread_mutex_destroy(&uart->tx_mutex);

    if (ret != 0) {
        return UART_ESYSTEM;
    }

    ret = pthread_mutex_destroy(&uart->rx_lock);

    if (ret != 0) {
        return UART_ESYSTEM;
    }

    ret = pthread_mutex_destroy(&uart->tx_lock);

    if (ret != 0) {
        return UART_ESYSTEM;
    }

    return UART_ESUCCESS;
}

void _uart_thread_lock_rx(uart_t *uart)
{
    pthread_mutex_lock(&uart->rx_lock);
}

void _uart_thread_lock_tx(uart_t *uart)
{
    pthread_mutex_lock(&uart->tx_lock);
}

void _uart_thread_unlock_rx(uart_t *uart)
{
    pthread_mutex_unlock(&uart->rx_lock);
}

void _uart_thread_unlock_tx(uart_t *uart)
{
    pthread_mutex_unlock(&uart->tx_lock);
}
