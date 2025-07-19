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

#include "_uart.h"
#include "_buffer.h"

#include <UART.h>

struct _thread_args {
    struct _uart_ctx *ctx;
    struct _uart *uart;
};

#define THREAD_SLEEP_1MS        1
#define THREAD_BUFFER_SIZE      4096

int _uart_thread_init(struct _uart_ctx *ctx, struct _uart *uart)
{
    HANDLE ret;

    if (!ctx) {
        return UART_ECTX;
    }

    if (!uart) {
        _uart_error(ctx, NULL, UART_EHANDLE, NULL, "NULL");

        return UART_EHANDLE;
    }

    ret = CreateMutexA(NULL, FALSE, "rx_mutex");

    if (ret == INVALID_HANDLE_VALUE) {
        _uart_error(ctx, uart, UART_ESYSAPI, "CreateMutexA", NULL);

        return UART_ESYSAPI;
    }

    uart->rx_mutex = ret;

    ret = CreateMutexA(NULL, FALSE, "tx_mutex");

    if (ret == INVALID_HANDLE_VALUE) {
        _uart_error(ctx, uart, UART_ESYSAPI, "CreateMutexA", NULL);

        return UART_ESYSAPI;
    }

    uart->tx_mutex = ret;

    ret = CreateMutexA(NULL, FALSE, "rx_lock");

    if (ret == INVALID_HANDLE_VALUE) {
        _uart_error(ctx, uart, UART_ESYSAPI, "CreateMutexA", NULL);

        return UART_ESYSAPI;
    }

    uart->rx_lock = ret;

    ret = CreateMutexA(NULL, FALSE, "tx_lock");

    if (ret == INVALID_HANDLE_VALUE) {
        _uart_error(ctx, uart, UART_ESYSAPI, "CreateMutexA", NULL);

        return UART_ESYSAPI;
    }

    uart->tx_lock = ret;

    return UART_ESUCCESS;
}

DWORD WINAPI worker_thread_rx(LPVOID lpParam)
{
    int run = 1;
    struct _thread_args *args = (struct _thread_args *) lpParam;
    ssize_t len;
    ssize_t ret;
    unsigned char buf[THREAD_BUFFER_SIZE];
    int bytes;
    int ret_ioctl;
    DWORD dwbytesread;
    COMSTAT comst;
    DWORD dwerror;

    while (run) {
        WaitForSingleObject(&args->uart->rx_lock, INFINITE);

        ret_ioctl = ClearCommError(args->uart->h, &dwerror, &comst);

        if (!ret_ioctl) {
            _uart_error(args->ctx, args->uart, UART_ESYSAPI, "ClearCommError", NULL);
            ReleaseMutex(&args->uart->rx_lock);
            WaitForSingleObject(&args->uart->rx_mutex, INFINITE);
            args->uart->rx_thread_run = 0;
            ReleaseMutex(&args->uart->rx_mutex);

            return 0;
        }

        ret = (int) comst.cbInQue;
        bytes = ret;
        len = buffer_get_free(args->uart->rx_buffer);

        if (len >= bytes) {
            ret = ReadFile(args->uart->h,
                           (LPVOID) buf,
                           (DWORD) len,
                           &dwbytesread,
                           NULL);

            if (ret == -1) {
                _uart_error(args->ctx, args->uart, UART_ESYSAPI, "read", NULL);
                ReleaseMutex(&args->uart->rx_lock);
                WaitForSingleObject(&args->uart->rx_mutex, INFINITE);
                args->uart->rx_thread_run = 0;
                ReleaseMutex(&args->uart->rx_mutex);

                return 0;
            }

            buffer_wr(args->uart->rx_buffer, buf, bytes);
        }

        ReleaseMutex(&args->uart->rx_lock);
        WaitForSingleObject(&args->uart->rx_mutex, INFINITE);

        if (!args->uart->rx_thread_run) {
            run = 0;
        }

        ReleaseMutex(&args->uart->rx_mutex);
        Sleep(THREAD_SLEEP_1MS);
    }

    return 0;
}

DWORD WINAPI worker_thread_tx(LPVOID lpParam)
{
    int run = 1;
    struct _thread_args *args = (struct _thread_args *) lpParam;
    ssize_t ret;
    unsigned char buf[THREAD_BUFFER_SIZE];
    ssize_t len;
    DWORD dwbytestowrite;
    DWORD dwbyteswritten;


    while (run) {
        WaitForSingleObject(&args->uart->tx_lock, INFINITE);
        len = buffer_get_num(args->uart->tx_buffer);
        dwbytestowrite = (DWORD) len;

        if (len >= THREAD_BUFFER_SIZE) {
            buffer_rd(args->uart->tx_buffer, buf, THREAD_BUFFER_SIZE);
            ret = WriteFile(args->uart->h,
                            (LPVOID) buf,
                            dwbytestowrite,
                            &dwbyteswritten,
                            NULL);

            if (!ret) {
                _uart_error(args->ctx, args->uart, UART_ESYSAPI, "WriteFile", NULL);
                ReleaseMutex(&args->uart->tx_lock);
                WaitForSingleObject(&args->uart->tx_mutex, INFINITE);
                args->uart->tx_thread_run = 0;
                ReleaseMutex(&args->uart->tx_mutex);

                return 0;
            }

            ret = (int) dwbyteswritten;

            if (ret != (ssize_t) len) {
                _uart_error(args->ctx, args->uart, UART_ESYSAPI, "WriteFile", "could not send all data");
                ReleaseMutex(&args->uart->tx_lock);
                WaitForSingleObject(&args->uart->tx_mutex, INFINITE);
                args->uart->tx_thread_run = 0;
                ReleaseMutex(&args->uart->tx_mutex);

                return 0;
            }
        } else if (len > 1) {
            buffer_rd(args->uart->tx_buffer, buf, len);
            ret = WriteFile(args->uart->h,
                            (LPVOID) buf,
                            dwbytestowrite,
                            &dwbyteswritten,
                            NULL);

            if (!ret) {
                _uart_error(args->ctx, args->uart, UART_ESYSAPI, "WriteFile", NULL);
                ReleaseMutex(&args->uart->tx_lock);
                WaitForSingleObject(&args->uart->tx_mutex, INFINITE);
                args->uart->tx_thread_run = 0;
                ReleaseMutex(&args->uart->tx_mutex);

                return 0;
            }

            ret = (int) dwbyteswritten;

            if (ret != (ssize_t) len) {
                _uart_error(args->ctx, args->uart, UART_ESYSAPI, "WriteFile", "could not send all data");
                ReleaseMutex(&args->uart->tx_lock);
                WaitForSingleObject(&args->uart->tx_mutex, INFINITE);
                args->uart->tx_thread_run = 0;
                ReleaseMutex(&args->uart->tx_mutex);

                return 0;
            }
        }

        ReleaseMutex(&args->uart->tx_lock);
        WaitForSingleObject(&args->uart->tx_mutex, INFINITE);

        if (!args->uart->tx_thread_run) {
            run = 0;
        }

        ReleaseMutex(&args->uart->tx_mutex);
        Sleep(THREAD_SLEEP_1MS);
    }

    return 0L;
}

int _uart_thread_start(struct _uart_ctx *ctx, struct _uart *uart)
{
    HANDLE ret;
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

    ret = CreateThread(NULL, 0, worker_thread_rx, (LPVOID) &args, 0, NULL);

    if (ret == NULL) {
        _uart_error(ctx, uart, UART_ESYSAPI, "CreateThread", NULL);

        return UART_ESYSAPI;
    }

    ret = CreateThread(NULL, 0, worker_thread_tx, (LPVOID) &args, 0, NULL);

    if (ret == NULL) {
        _uart_error(ctx, uart, UART_ESYSAPI, "CreateThread", NULL);

        return UART_ESYSAPI;
    }

    return UART_ESUCCESS;
}

int _uart_thread_stop(struct _uart_ctx *ctx, struct _uart *uart)
{
    BOOL ret;

    if (!ctx) {
        return UART_ECTX;
    }

    if (!uart) {
        _uart_error(ctx, NULL, UART_EHANDLE, NULL, "NULL");

        return UART_EHANDLE;
    }

    ret = WaitForSingleObject(uart->rx_lock, INFINITE);

    if (ret == 0) {
        _uart_error(ctx, uart, UART_ESYSAPI, "WaitForSingleObject", NULL);

        return UART_ESYSAPI;
    }

    uart->rx_thread_run = 0;
    ret = ReleaseMutex(uart->rx_mutex);

    if (ret == 0) {
        _uart_error(ctx, uart, UART_ESYSAPI, "ReleaseMutex", NULL);

        return UART_ESYSAPI;
    }

    ret = WaitForSingleObject(uart->tx_lock, INFINITE);

    if (ret == 0) {
        _uart_error(ctx, uart, UART_ESYSAPI, "WaitForSingleObject", NULL);

        return UART_ESYSAPI;
    }

    uart->tx_thread_run = 0;
    ret = ReleaseMutex(uart->tx_mutex);

    if (ret == 0) {
        _uart_error(ctx, uart, UART_ESYSAPI, "ReleaseMutex", NULL);

        return UART_ESYSAPI;
    }

    ret = WaitForSingleObject(uart->rx_thread, INFINITE);

    if (ret == 0) {
        _uart_error(ctx, uart, UART_ESYSAPI, "WaitForSingleObject", NULL);

        return UART_ESYSAPI;
    }

    ret = WaitForSingleObject(uart->tx_thread, INFINITE);

    if (ret == 0) {
        _uart_error(ctx, uart, UART_ESYSAPI, "WaitForSingleObject", NULL);

        return UART_ESYSAPI;
    }

    ret = CloseHandle(uart->rx_mutex);

    if (ret == 0) {
        _uart_error(ctx, uart, UART_ESYSAPI, "CloseHandle", NULL);

        return UART_ESYSAPI;
    }

    ret = CloseHandle(uart->tx_mutex);

    if (ret == 0) {
        _uart_error(ctx, uart, UART_ESYSAPI, "CloseHandle", NULL);

        return UART_ESYSAPI;
    }

    ret = CloseHandle(uart->rx_lock);

    if (ret == 0) {
        _uart_error(ctx, uart, UART_ESYSAPI, "CloseHandle", NULL);

        return UART_ESYSAPI;
    }

    ret = CloseHandle(uart->tx_lock);

    if (ret == 0) {
        _uart_error(ctx, uart, UART_ESYSAPI, "CloseHandle", NULL);

        return UART_ESYSAPI;
    }

    ret = CloseHandle(uart->rx_thread);

    if (ret == 0) {
        _uart_error(ctx, uart, UART_ESYSAPI, "CloseHandle", NULL);

        return UART_ESYSAPI;
    }

    ret = CloseHandle(uart->tx_thread);

    if (ret == 0) {
        _uart_error(ctx, uart, UART_ESYSAPI, "CloseHandle", NULL);

        return UART_ESYSAPI;
    }

    return UART_ESUCCESS;
}

int _uart_thread_lock_rx(struct _uart_ctx *ctx, struct _uart *uart)
{
    BOOL ret;

    if (!ctx) {
        return UART_ECTX;
    }

    if (!uart) {
        _uart_error(ctx, NULL, UART_EHANDLE, NULL, "NULL");

        return UART_EHANDLE;
    }

    ret = WaitForSingleObject(uart->rx_lock, INFINITE);

    if (ret == 0) {
        _uart_error(ctx, uart, UART_ESYSAPI, "WaitForSingleObject", NULL);

        return UART_ESYSAPI;
    }

    return UART_ESUCCESS;
}

int _uart_thread_lock_tx(struct _uart_ctx *ctx, struct _uart *uart)
{
    BOOL ret;

    if (!ctx) {
        return UART_ECTX;
    }

    if (!uart) {
        _uart_error(ctx, NULL, UART_EHANDLE, NULL, "NULL");

        return UART_EHANDLE;
    }

    ret = WaitForSingleObject(uart->tx_lock, INFINITE);

    if (ret == 0) {
        _uart_error(ctx, uart, UART_ESYSAPI, "WaitForSingleObject", NULL);

        return UART_ESYSAPI;
    }

    return UART_ESUCCESS;
}

int _uart_thread_unlock_rx(struct _uart_ctx *ctx, struct _uart *uart)
{
    BOOL ret;

    if (!ctx) {
        return UART_ECTX;
    }

    if (!uart) {
        _uart_error(ctx, NULL, UART_EHANDLE, NULL, "NULL");

        return UART_EHANDLE;
    }

    ret = ReleaseMutex(uart->rx_lock);

    if (ret == 0) {
        _uart_error(ctx, uart, UART_ESYSAPI, "ReleaseMutex", NULL);

        return UART_ESYSAPI;
    }

    return UART_ESUCCESS;
}

int _uart_thread_unlock_tx(struct _uart_ctx *ctx, struct _uart *uart)
{
    BOOL ret;

    if (!ctx) {
        return UART_ECTX;
    }

    if (!uart) {
        _uart_error(ctx, NULL, UART_EHANDLE, NULL, "NULL");

        return UART_EHANDLE;
    }

    ret = ReleaseMutex(uart->tx_lock);

    if (ret == 0) {
        _uart_error(ctx, uart, UART_ESYSAPI, "ReleaseMutex", NULL);

        return UART_ESYSAPI;
    }

    return UART_ESUCCESS;
}
