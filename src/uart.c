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

#include <stdlib.h>
#include <string.h>

#include "_uart.h"
#include "_version.h"
#include "_util.h"

#ifdef LIBUART_THREADS
#include "_buffer.h"
#include "_thread.h"
#endif

#include <UART.h>

static int parse_option(uart_ctx_t *ctx, uart_t *uart, const char *opt)
{
    int i = 0;
    
    if (!ctx) {
        return UART_ECTX;
    }

    if (!uart) {
        _uart_error(ctx, NULL, UART_EHANDLE, NULL, "NULL");

        return UART_EHANDLE;
    }

    while (opt[i] != '\0') {
        /* parse data bits */
        switch (opt[i]) {
        case '5':
            uart->data_bits = UART_DATA_5;
            break;
        case '6':
            uart->data_bits = UART_DATA_6;
            break;
        case '7':
            uart->data_bits = UART_DATA_7;
            break;
        case '8':
            uart->data_bits = UART_DATA_8;
            break;
        default:
            _uart_error(ctx, uart, UART_EDATA, NULL, "unsupported");

            return UART_EDATA;
        }
        
        i++;
        
        /* parse parity */
        switch (opt[i]) {
        case 'N':
            uart->parity = UART_PARITY_NONE;
            break;
        case 'O':
            uart->parity = UART_PARITY_ODD;
            break;
        case 'E':
            uart->parity = UART_PARITY_EVEN;
            break;
        default:
            _uart_error(ctx, uart, UART_EPARITY, NULL, "unsupported");

            return UART_EPARITY;
        }
        
        i++;
        
        /* parse stop bits */
        switch (opt[i]) {
        case '1':
            uart->stop_bits = UART_STOP_1_0;
            break;
        case '2':
            uart->stop_bits = UART_STOP_2_0;
            break;
        default:
            _uart_error(ctx, uart, UART_ESTOP, NULL, "unsupported");

            return UART_ESTOP;
        }
        
        i++;
        
        /* parse flow control */
        switch (opt[i]) {
        case 'N':
            uart->flow_ctrl = UART_FLOW_NO;
            break;
        case 'S':
            uart->flow_ctrl = UART_FLOW_SW;
            break;
        case 'H':
            uart->flow_ctrl = UART_FLOW_HW;
            break;
        default:
            _uart_error(ctx, uart, UART_EFLOW, NULL, "unsupported");

            return UART_EFLOW;
        }
        
        i++;
        
        if (opt[i] != '\0') {
            _uart_error(ctx, uart, UART_EOPT, NULL, NULL);

            return UART_EOPT;
        }
    }
    
    return UART_ESUCCESS;
}

int UART_init(uart_ctx_t **ret_ctx)
{
    int ret;
    uart_ctx_t *ctx;

    ctx = (uart_ctx_t *) malloc(sizeof(uart_ctx_t));

    if (!ctx) {
        _uart_error(NULL, NULL, UART_ENOMEM, NULL, NULL);

        return UART_ENOMEM;
    }

    memset(ctx, 0, sizeof(uart_ctx_t));
    ctx->errormsg = (char *) malloc(UART_ERRORMAX);

    if (!ctx->errormsg) {
        free(ctx);
        _uart_error(NULL, NULL, UART_ENOMEM, NULL, NULL);

        return UART_ENOMEM;
    }

    memset(ctx->errormsg, 0, UART_ERRORMAX);
    ret = _uart_init(ctx);

    if (ret != UART_ESUCCESS) {
        return ret;
    }

    ctx->init_done = 1;
    ctx->flags |= UART_CTXFOKAY;
    *(ret_ctx) = ctx;
    ret = _uart_get_device_list(ctx);

    if (ret != UART_ESUCCESS) {
        return ret;
    }

    return UART_ESUCCESS;
}

int UART_free(uart_ctx_t *ctx)
{
    int ret;
    int i;

    if (!ctx) {
        return UART_ECTX;
    }

    for (i = 0; i < ctx->uarts_count; i++) {
        ret = UART_dev_close(ctx, ctx->uarts[i]);

        if (ret != UART_ESUCCESS) {
            return ret;
        }

        ret = UART_dev_free(ctx, ctx->uarts[i]);

        if (ret != UART_ESUCCESS) {
            return ret;
        }
    }

    if (ctx->errormsg) {
        free(ctx->errormsg);
    }

    free(ctx);

    return UART_ESUCCESS;
}

ssize_t UART_get_device_list(uart_ctx_t *ctx, uart_t **ret_uarts)
{
    int ret;
    struct _uart *tmp_uarts[UART_DEVMAX];
    int tmp_uarts_count = 0;
    int i;
    int j;

    if (!ctx) {
        return UART_ECTX;
    }

    for (i = 0; i < ctx->uarts_count; i++) {
        if (ctx->uarts[i]->flags & UART_FOPENED) {
            tmp_uarts[tmp_uarts_count] = ctx->uarts[i];
            tmp_uarts_count++;
        } else {
            free(ctx->uarts[i]->errormsg);
            free(ctx->uarts[i]);
        }
    }

    ret = _uart_get_device_list(ctx);

    if (ret != UART_ESUCCESS) {
        return ret;
    }

    if (tmp_uarts_count) {
        for (i = 0; i < ctx->uarts_count; i++) {
            for (j = 0; j < tmp_uarts_count; j++) {
                if (strcmp(tmp_uarts[j]->dev, ctx->uarts[i]->dev) == 0) {
                    ctx->uarts[i]->flags |= UART_FOPENED;
                }
            }
        }
    }

    *(ret_uarts) = (uart_t *) ctx->uarts;

    return (ssize_t) ctx->uarts_count;
}

uart_t *UART_dev_open_name(uart_ctx_t *ctx, const char *devname, enum e_baud baud, const char *opt)
{
    uart_t *uart;
    int ret;
    int i;
    int found = 0;

    if (!ctx) {
        return NULL;
    }

    if (!devname) {
        _uart_error(ctx, NULL, UART_EDEV, NULL, "NULL");

        return NULL;
    }

    if (ctx->uarts_count) {
        for (i = 0; i < ctx->uarts_count; i++) {
            if (strcmp(devname, ctx->uarts[i]->dev) == 0) {
                if (ctx->uarts[i]->flags & UART_FOPENED) {
                    return ctx->uarts[i];
                } else {
                    ret = parse_option(ctx, ctx->uarts[i], opt);

                    if (ret != UART_ESUCCESS) {
                        return NULL;
                    }

                    ctx->uarts[i]->baud = baud;
                    ret = _uart_open(ctx, ctx->uarts[i]);

                    if (ret != UART_ESUCCESS) {
                        return NULL;
                    }

                    uart = ctx->uarts[i];
                    found = 1;
                    break;
                }
            }
        }

        if (!found) {
            uart = (uart_t *) malloc(sizeof(uart_t));

            if (!uart) {
                _uart_error(ctx, NULL, UART_ENOMEM, NULL, NULL);

                return NULL;
            }

            memset(uart, 0, sizeof(uart_t));
            uart->errormsg = (char *) malloc(UART_ERRORMAX);

            if (!uart->errormsg) {
                _uart_error(ctx, uart, UART_ENOMEM, NULL, NULL);

                return NULL;
            }

            ctx->uarts[ctx->uarts_count] = uart;
            ctx->uarts_count++;
            memset(uart->errormsg, 0, UART_ERRORMAX);
            strcpy(uart->dev, devname);
            ret = parse_option(ctx, uart, opt);

            if (ret != UART_ESUCCESS) {
                free(uart);

                return NULL;
            }

            uart->baud = baud;
            ret = _uart_open(ctx, uart);

            if (ret != UART_ESUCCESS) {
                free(uart);

                return NULL;
            }
        }
    } else {
        uart = (uart_t *) malloc(sizeof(uart_t));

        if (!uart) {
            _uart_error(ctx, NULL, UART_ENOMEM, NULL, NULL);

            return NULL;
        }

        memset(uart, 0, sizeof(uart_t));
        uart->errormsg = (char *) malloc(UART_ERRORMAX);

        if (!uart->errormsg) {
            _uart_error(ctx, uart, UART_ENOMEM, NULL, NULL);

            return NULL;
        }

        ctx->uarts[ctx->uarts_count] = uart;
        ctx->uarts_count++;
        memset(uart->errormsg, 0, UART_ERRORMAX);
        strcpy(uart->dev, devname);
        ret = parse_option(ctx, uart, opt);

        if (ret != UART_ESUCCESS) {
            free(uart);

            return NULL;
        }

        uart->baud = baud;
        ret = _uart_open(ctx, uart);

        if (ret != UART_ESUCCESS) {
            free(uart);

            return NULL;
        }
    }

#ifdef LIBUART_THREADS
    if (!(uart->flags & UART_FOPENED)) {
        uart->rx_buffer = buffer_create(UART_BUFFERSIZE);

        if (!uart->rx_buffer) {
            _uart_error(ctx, uart, UART_EBUF, NULL, NULL);

            return NULL;
        }

        uart->tx_buffer = buffer_create(UART_BUFFERSIZE);

        if (!uart->tx_buffer) {
            _uart_error(ctx, uart, UART_EBUF, NULL, NULL);

            return NULL;
        }

        ret = _uart_thread_init(ctx, uart);

        if (ret != UART_ESUCCESS) {
            return NULL;
        }

        ret = _uart_thread_start(ctx, uart);

        if (ret != UART_ESUCCESS) {
            return NULL;
        }
    }
#endif

    uart->flags |= UART_FOPENED;

    return uart;
}

int UART_dev_open(uart_ctx_t *ctx, uart_t *uart, enum e_baud baud, const char *opt)
{
    int ret;
    int i;
    int found = 0;

    if (!ctx) {
        return UART_ECTX;
    }

    if (!uart) {
        _uart_error(ctx, NULL, UART_EHANDLE, NULL, "NULL");

        return UART_EHANDLE;
    }

    if (ctx->uarts_count) {
        for (i = 0; i < ctx->uarts_count; i++) {
            if (ctx->uarts[i] == uart) {
                found = 1;
                break;
            }
        }

        if (!found) {
            _uart_error(ctx, NULL, UART_EDEV, NULL, NULL);

            return UART_EDEV;
        }
    } else {
        _uart_error(ctx, NULL, UART_EDEV, NULL, NULL);

        return UART_EDEV;
    }

    if (!(uart->flags & UART_FOPENED)) {
        ret = parse_option(ctx, uart, opt);

        if (ret != UART_ESUCCESS) {
            free(uart);

            return ret;
        }

        uart->baud = baud;
        ret = _uart_open(ctx, uart);

        if (ret != UART_ESUCCESS) {
            return ret;
        }
    }

#ifdef LIBUART_THREADS
    if (!(uart->flags & UART_FOPENED)) {
        uart->rx_buffer = buffer_create(UART_BUFFERSIZE);

        if (!uart->rx_buffer) {
            _uart_error(ctx, uart, UART_EBUF, NULL, NULL);

            return UART_EBUF;
        }

        uart->tx_buffer = buffer_create(UART_BUFFERSIZE);

        if (!uart->tx_buffer) {
            _uart_error(ctx, uart, UART_EBUF, NULL, NULL);

            return UART_EBUF;
        }

        ret = _uart_thread_init(ctx, uart);

        if (ret != UART_ESUCCESS) {
            return ret;
        }

        ret = _uart_thread_start(ctx, uart);

        if (ret != UART_ESUCCESS) {
            return ret;
        }
    }
#endif

    uart->flags |= UART_FOPENED;

    return UART_ESUCCESS;
}

int UART_dev_close(uart_ctx_t *ctx, uart_t *uart)
{
    int ret;

    if (!ctx) {
        return UART_ECTX;
    }

    if (!uart) {
        _uart_error(ctx, NULL, UART_EHANDLE, NULL, "NULL");

        return UART_EHANDLE;
    }

    if (uart->flags & UART_FOPENED) {
#ifdef LIBUART_THREADS
        ret = _uart_thread_stop(ctx, uart);

        if (ret != UART_ESUCCESS) {
            return ret;
        }

        buffer_free(uart->rx_buffer);
        buffer_free(uart->tx_buffer);
#endif

        ret = _uart_flush(ctx, uart);

        if (ret != UART_ESUCCESS) {
            return ret;
        }

        ret = _uart_close(ctx, uart);

        if (ret != UART_ESUCCESS) {
            return ret;
        }

        uart->flags &= ~(UART_FOPENED);
    }

    return UART_ESUCCESS;
}

int UART_dev_free(uart_ctx_t *ctx, uart_t *uart)
{
    int ret;

    if (!ctx) {
        return UART_ECTX;
    }

    if (!uart) {
        _uart_error(ctx, NULL, UART_EHANDLE, NULL, "NULL");

        return UART_EHANDLE;
    }

    ret = UART_dev_close(ctx, uart);

    if (ret != UART_ESUCCESS) {
        return ret;
    }

    free(uart->errormsg);
    free(uart);

    return UART_ESUCCESS;
}

ssize_t UART_send(uart_ctx_t *ctx, uart_t *uart, void *send_buf, size_t len)
{
    ssize_t ret;

    if (!ctx) {
        return UART_ECTX;
    }

    if (!uart) {
        _uart_error(ctx, NULL, UART_EHANDLE, NULL, "NULL");

        return UART_EHANDLE;
    }

#ifndef LIBUART_THREADS
    ret = _uart_send(ctx, uart, send_buf, len);
#else
    _uart_thread_lock_tx(ctx, uart);

    if (buffer_get_free(uart->tx_buffer) >= (ssize_t) len) {
        ret = buffer_wr(uart->tx_buffer, send_buf, (ssize_t) len);
    } else {
        _uart_error(ctx, uart, UART_EBUF, NULL, "full");

        return UART_EBUF;
    }

    _uart_thread_unlock_tx(ctx, uart);
#endif

    return ret;
}

ssize_t UART_recv(uart_ctx_t *ctx, uart_t *uart, void *recv_buf, size_t len)
{
    ssize_t ret;

    if (!ctx) {
        return UART_ECTX;
    }

    if (!uart) {
        _uart_error(ctx, NULL, UART_EHANDLE, NULL, "NULL");

        return UART_EHANDLE;
    }

#ifndef LIBUART_THREADS
    ret = _uart_recv(ctx, uart, recv_buf, len);
#else
    _uart_thread_lock_rx(ctx, uart);

    if (buffer_get_num(uart->rx_buffer) == 0) {
        return 0;
    } else {
        if (buffer_get_num(uart->rx_buffer) < (ssize_t) len) {
            ret = buffer_rd(uart->rx_buffer, recv_buf, buffer_get_num(uart->rx_buffer));
        } else {
            ret = buffer_rd(uart->rx_buffer, recv_buf, (ssize_t) len);
        }
    }

    _uart_thread_unlock_rx(ctx, uart);
#endif

    return ret;
}

ssize_t UART_puts(uart_ctx_t *ctx, uart_t *uart, char *msg)
{
    if (!ctx) {
        return UART_ECTX;
    }

    if (!uart) {
        _uart_error(ctx, NULL, UART_EHANDLE, NULL, "NULL");

        return UART_EHANDLE;
    }

    return UART_send(ctx, uart, msg, strlen(msg));
}

int UART_putc(uart_ctx_t *ctx, uart_t *uart, char c)
{
    ssize_t ret;

    if (!ctx) {
        return UART_ECTX;
    }

    if (!uart) {
        _uart_error(ctx, NULL, UART_EHANDLE, NULL, "NULL");

        return UART_EHANDLE;
    }

    ret = UART_send(ctx, uart, &c, 1);

    if (ret != 1) {
        return ret;
    }

    return UART_ESUCCESS;
}

int UART_getc(uart_ctx_t *ctx, uart_t *uart, char *ret_c)
{
    ssize_t ret;
    char c[1];

    if (!ctx) {
        return UART_ECTX;
    }

    if (!uart) {
        _uart_error(ctx, NULL, UART_EHANDLE, NULL, "NULL");

        return UART_EHANDLE;
    }

    ret = UART_recv(ctx, uart, c, 1);

    if (ret != 1) {
        return ret;
    }

    *(ret_c) = c[0];

    return UART_ESUCCESS;
}

int UART_flush(uart_ctx_t *ctx, uart_t *uart)
{
    int ret;

    if (!ctx) {
        return UART_ECTX;
    }

    if (!uart) {
        _uart_error(ctx, NULL, UART_EHANDLE, NULL, "NULL");

        return UART_EHANDLE;
    }

    ret = _uart_flush(ctx, uart);

    if (ret != UART_ESUCCESS) {
        return ret;
    }

    return UART_ESUCCESS;
}

int UART_set_pin(uart_ctx_t *ctx, uart_t *uart, enum e_pins pin, int state)
{
    int ret;

    if (!ctx) {
        return UART_ECTX;
    }

    if (!uart) {
        _uart_error(ctx, NULL, UART_EHANDLE, NULL, "NULL");

        return UART_EHANDLE;
    }

    ret = _uart_set_pin(ctx, uart, pin, state);

    if (ret != UART_ESUCCESS) {
        return ret;
    }

    return UART_ESUCCESS;
}

int UART_get_pin(uart_ctx_t *ctx, uart_t *uart, enum e_pins pin, int *ret_state)
{
    int ret;

    if (!ctx) {
        return UART_ECTX;
    }

    if (!uart) {
        _uart_error(ctx, NULL, UART_EHANDLE, NULL, "NULL");

        return UART_EHANDLE;
    }

    if (!ret_state) {
        _uart_error(ctx, uart, UART_EINVAL, NULL, "int pointer (NULL)");

        return UART_EINVAL;
    }

    ret = _uart_get_pin(ctx, uart, pin, ret_state);

    if (ret != UART_ESUCCESS) {
        return ret;
    }

    return UART_ESUCCESS;
}

int UART_set_baud(uart_ctx_t *ctx, uart_t *uart, enum e_baud baud)
{
    int ret;

    if (!ctx) {
        return UART_ECTX;
    }

    if (!uart) {
        _uart_error(ctx, NULL, UART_EHANDLE, NULL, "NULL");

        return UART_EHANDLE;
    }

    ret = _uart_baud_valid((int) baud);

    if (ret == 0) {
        _uart_error(ctx, uart, UART_EBAUD, NULL, NULL);

        return UART_EBAUD;
    }

    uart->baud = baud;
    ret = _uart_init_baud(ctx, uart);

    if (ret != UART_ESUCCESS) {
        return ret;
    }

    return UART_ESUCCESS;
}

int UART_get_baud(uart_ctx_t *ctx, uart_t *uart, int *ret_baud)
{
    if (!ctx) {
        return UART_ECTX;
    }

    if (!uart) {
        _uart_error(ctx, NULL, UART_EHANDLE, NULL, "NULL");

        return UART_EHANDLE;
    }

    if (!ret_baud) {
        _uart_error(ctx, uart, UART_EINVAL, NULL, "int pointer (NULL)");

        return UART_EINVAL;
    }

    *(ret_baud) = (int) uart->baud;

    return UART_ESUCCESS;
}

int UART_set_databits(uart_ctx_t *ctx, uart_t *uart, enum e_data data_bits)
{
    int ret;

    if (!ctx) {
        return UART_ECTX;
    }

    if (!uart) {
        _uart_error(ctx, NULL, UART_EHANDLE, NULL, "NULL");

        return UART_EHANDLE;
    }

    uart->data_bits = data_bits;
    ret = _uart_init_databits(ctx, uart);

    if (ret != UART_ESUCCESS) {
        return ret;
    }

    return UART_ESUCCESS;
}

int UART_get_databits(uart_ctx_t *ctx, uart_t *uart, int *ret_data_bits)
{
    if (!ctx) {
        return UART_ECTX;
    }

    if (!uart) {
        _uart_error(ctx, NULL, UART_EHANDLE, NULL, "NULL");

        return UART_EHANDLE;
    }

    if (!ret_data_bits) {
        _uart_error(ctx, uart, UART_EINVAL, NULL, "int pointer (NULL)");

        return UART_EINVAL;
    }

    *(ret_data_bits) = (int) uart->data_bits;

    return UART_ESUCCESS;
}

int UART_set_parity(uart_ctx_t *ctx, uart_t *uart, enum e_parity parity)
{
    int ret;

    if (!ctx) {
        return UART_ECTX;
    }

    if (!uart) {
        _uart_error(ctx, NULL, UART_EHANDLE, NULL, "NULL");

        return UART_EHANDLE;
    }

    uart->parity = parity;
    ret = _uart_init_parity(ctx, uart);

    if (ret != UART_ESUCCESS) {
        return ret;
    }

    return UART_ESUCCESS;
}

int UART_get_parity(uart_ctx_t *ctx, uart_t *uart, int *ret_parity)
{
    if (!ctx) {
        return UART_ECTX;
    }

    if (!uart) {
        _uart_error(ctx, NULL, UART_EHANDLE, NULL, "NULL");

        return UART_EHANDLE;
    }

    if (!ret_parity) {
        _uart_error(ctx, uart, UART_EINVAL, NULL, "int pointer (NULL)");

        return UART_EINVAL;
    }

    *(ret_parity) = (int) uart->parity;

    return UART_ESUCCESS;
}

int UART_set_stopbits(uart_ctx_t *ctx, uart_t *uart, enum e_stop stop_bits)
{
    int ret;

    if (!ctx) {
        return UART_ECTX;
    }

    if (!uart) {
        _uart_error(ctx, NULL, UART_EHANDLE, NULL, "NULL");

        return UART_EHANDLE;
    }

    uart->stop_bits = stop_bits;
    ret = _uart_init_stopbits(ctx, uart);

    if (ret != UART_ESUCCESS) {
        return ret;
    }

    return UART_ESUCCESS;
}

int UART_get_stopbits(uart_ctx_t *ctx, uart_t *uart, int *ret_stop_bits)
{
    if (!ctx) {
        return UART_ECTX;
    }

    if (!uart) {
        _uart_error(ctx, NULL, UART_EHANDLE, NULL, "NULL");

        return UART_EHANDLE;
    }

    if (!ret_stop_bits) {
        _uart_error(ctx, uart, UART_EINVAL, NULL, "int pointer (NULL)");

        return UART_EINVAL;
    }

    *(ret_stop_bits) = (int) uart->stop_bits;

    return UART_ESUCCESS;
}

int UART_set_flowctrl(uart_ctx_t *ctx, uart_t *uart, enum e_flow flow_ctrl)
{
    int ret;

    if (!ctx) {
        return UART_ECTX;
    }

    if (!uart) {
        _uart_error(ctx, NULL, UART_EHANDLE, NULL, "NULL");

        return UART_EHANDLE;
    }

    uart->flow_ctrl = flow_ctrl;
    ret = _uart_init_flow(ctx, uart);

    if (ret != UART_ESUCCESS) {
        return ret;
    }

    return UART_ESUCCESS;
}

int UART_get_flowctrl(uart_ctx_t *ctx, uart_t *uart, int *ret_flow_ctrl)
{
    if (!ctx) {
        return UART_ECTX;
    }

    if (!uart) {
        _uart_error(ctx, NULL, UART_EHANDLE, NULL, "NULL");

        return UART_EHANDLE;
    }

    if (!ret_flow_ctrl) {
        _uart_error(ctx, uart, UART_EINVAL, NULL, "int pointer (NULL)");

        return UART_EINVAL;
    }

    *(ret_flow_ctrl) = (int) uart->flow_ctrl;

    return UART_ESUCCESS;
}

#ifdef __unix__
int UART_get_fd(uart_ctx_t *ctx, uart_t *uart, int *ret_fd)
{
    if (!ctx) {
        return UART_ECTX;
    }

    if (!uart) {
        _uart_error(ctx, NULL, UART_EHANDLE, NULL, "NULL");

        return UART_EHANDLE;
    }

    if (!ret_fd) {
        _uart_error(ctx, uart, UART_EINVAL, NULL, "invalid file descriptor (NULL)");

        return UART_EINVAL;
    }

    *(ret_fd) = uart->fd;

    return UART_ESUCCESS;
}
#endif

#ifdef _WIN32
int UART_get_handle(uart_ctx_t *ctx, uart_t *uart, HANDLE *ret_h)
{
    if (!ctx) {
        return UART_ECTX;
    }

    if (!uart) {
        _uart_error(ctx, NULL, UART_EHANDLE, NULL, "NULL");

        return UART_EHANDLE;
    }

    if (!ret_h) {
        _uart_error(ctx, uart, UART_EINVAL, NULL, "invalid handle (NULL)");

        return UART_EINVAL;
    }

    *(ret_h) = uart->h;

    return UART_ESUCCESS;
}
#endif

int UART_get_dev(uart_ctx_t *ctx, uart_t *uart, char **ret_dev)
{
    if (!ctx) {
        return UART_ECTX;
    }

    if (!uart) {
        _uart_error(ctx, NULL, UART_EHANDLE, NULL, "NULL");

        return UART_EHANDLE;
    }

    if (!ret_dev) {
        _uart_error(ctx, uart, UART_EINVAL, NULL, "char pointer (NULL)");

        return UART_EINVAL;
    }

    *(ret_dev) = (char *) uart->dev;

    return UART_ESUCCESS;
}

int UART_get_bytes_available(uart_ctx_t *ctx, uart_t *uart, int *ret_num)
{
    int ret;

    if (!ctx) {
        return UART_ECTX;
    }

    if (!uart) {
        _uart_error(ctx, NULL, UART_EHANDLE, NULL, "NULL");

        return UART_EHANDLE;
    }

    if (!ret_num) {
        _uart_error(ctx, uart, UART_EINVAL, NULL, "int pointer (NULL)");

        return UART_EINVAL;
    }

#ifndef LIBUART_THREADS
    ret = _uart_get_bytes(ctx, uart, ret_num);

    if (ret != UART_ESUCCESS) {
        return ret;
    }
#else
    _uart_thread_lock_rx(ctx, uart);
    ret = (int) buffer_get_num(uart->rx_buffer);
    _uart_thread_unlock_rx(ctx, uart);

    *(ret_num) = ret;
#endif

    return UART_ESUCCESS;
}

int UART_get_deverrro(uart_ctx_t *ctx, uart_t *uart)
{
    if (!ctx) {
        return UART_ECTX;
    }

    if (!uart) {
        _uart_error(ctx, NULL, UART_EHANDLE, NULL, "NULL");

        return UART_EHANDLE;
    }

    return uart->error;
}

char *UART_get_deverrormsg(uart_ctx_t *ctx, uart_t *uart)
{
    if (!ctx) {
        return NULL;
    }

    if (!uart) {
        _uart_error(ctx, NULL, UART_EHANDLE, NULL, "NULL");

        return NULL;
    }

    return uart->errormsg;
}

int UART_get_ctxerrro(uart_ctx_t *ctx)
{
    if (!ctx) {
        return UART_ECTX;
    }

    return ctx->error;
}

char *UART_get_ctxerrormsg(uart_ctx_t *ctx)
{
    if (!ctx) {
        return NULL;
    }

    return ctx->errormsg;
}

char *UART_get_libname(void)
{
    return LIBUART_NAME;
}

char *UART_get_libversion(void)
{
#ifndef GIT_VERSION
    return LIBUART_VERSION;
#else
    return LIBUART_VERSION " (git-" TOSTRING(GIT_VERSION) ")";
#endif
}
