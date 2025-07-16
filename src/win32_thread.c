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

#include <stddef.h>
#include <windows.h>

#include "system.h"

#include <UART.h>

int _uart_thread_init(uart_t *uart)
{
    HANDLE ret;

    ret = CreateMutexA(NULL, TRUE, "rx_mutex");

    if (ret == NULL) {
        return UART_ESYSTEM;
    }

    uart->rx_mutex = ret;

    ret = CreateMutexA(NULL, TRUE, "tx_mutex");

    if (ret == NULL) {
        return UART_ESYSTEM;
    }

    uart->tx_mutex = ret;

    ret = CreateMutexA(NULL, TRUE, "rx_lock");

    if (ret == NULL) {
        return UART_ESYSTEM;
    }

    uart->rx_lock = ret;

    ret = CreateMutexA(NULL, TRUE, "tx_lock");

    if (ret == NULL) {
        return UART_ESYSTEM;
    }

    uart->tx_lock = ret;

    return UART_ESUCCESS;
}

DWORD WINAPI worker_thread_rx(LPVOID lpParam)
{
    int run = 1;
    uart_t *uart = (uart_t *) lpParam;

    while (run) {
        Sleep(1);
        WaitForSingleObject(uart->rx_lock, 5000);

        // TODO

        ReleaseMutex(uart->rx_lock);

        WaitForSingleObject(uart->rx_mutex, 5000);

        if (!uart->rx_thread_run) {
            run = 0;
        }

        ReleaseMutex(uart->rx_mutex);
    }

    return 0;
}

DWORD WINAPI worker_thread_tx(LPVOID lpParam)
{
    int run = 1;
    uart_t *uart = (uart_t *) lpParam;

    while (run) {
        Sleep(1);
        WaitForSingleObject(uart->tx_lock, 5000);

        // TODO

        ReleaseMutex(uart->tx_lock);

        WaitForSingleObject(uart->tx_mutex, 5000);

        if (!uart->tx_thread_run) {
            run = 0;
        }

        ReleaseMutex(uart->tx_mutex);
    }

    return 0;
}

int _uart_thread_start(uart_t *uart)
{
    HANDLE ret;

    uart->rx_thread_run = 1;
    uart->tx_thread_run = 1;

    ret = CreateThread(NULL, 0, worker_thread_rx, (LPVOID) uart, 0, NULL);

    if (ret == NULL) {
        return UART_ESYSTEM;
    }

    ret = CreateThread(NULL, 0, worker_thread_tx, (LPVOID) uart, 0, NULL);

    if (ret == NULL) {
        return UART_ESYSTEM;
    }

    return UART_ESUCCESS;
}

int _uart_thread_stop(uart_t *uart)
{
    BOOL ret;

    WaitForSingleObject(uart->rx_lock, 5000);
    uart->rx_thread_run = 0;
    ReleaseMutex(uart->rx_mutex);

    WaitForSingleObject(uart->tx_lock, 5000);
    uart->tx_thread_run = 0;
    ReleaseMutex(uart->tx_mutex);

    WaitForSingleObject(uart->rx_thread, 5000);
    WaitForSingleObject(uart->tx_thread, 5000);

    ret = CloseHandle(uart->rx_mutex);

    if (ret == 0) {
        return UART_ESYSTEM;
    }

    ret = CloseHandle(uart->tx_mutex);

    if (ret == 0) {
        return UART_ESYSTEM;
    }

    ret = CloseHandle(uart->rx_lock);

    if (ret == 0) {
        return UART_ESYSTEM;
    }

    ret = CloseHandle(uart->tx_lock);

    if (ret == 0) {
        return UART_ESYSTEM;
    }

    ret = CloseHandle(uart->rx_thread);

    if (ret == 0) {
        return UART_ESYSTEM;
    }

    ret = CloseHandle(uart->tx_thread);

    if (ret == 0) {
        return UART_ESYSTEM;
    }

    return UART_ESUCCESS;
}

void _uart_thread_lock_rx(uart_t *uart)
{
    BOOL ret;

    ret = WaitForSingleObject(uart->rx_lock, 5000);

    if (ret == 0) {
        return;
    }
}

void _uart_thread_lock_tx(uart_t *uart)
{
    BOOL ret;

    ret = WaitForSingleObject(uart->tx_lock, 5000);

    if (ret == 0) {
        return;
    }
}

void _uart_thread_unlock_rx(uart_t *uart)
{
    BOOL ret;

    ret = ReleaseMutex(uart->rx_lock);

    if (ret == 0) {
        return;
    }
}

void _uart_thread_unlock_tx(uart_t *uart)
{
    BOOL ret;

    ret = ReleaseMutex(uart->tx_lock);

    if (ret == 0) {
        return;
    }
}
