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

#include "error.h"
#include "system.h"
#include "version.h"
#include "util.h"

#ifdef LIBUART_THREADS
#include "buffer.h"
#include "thread.h"
#endif

#include <UART.h>

static int uart_init_done = 0;

static int parse_option(uart_t *uart, const char *opt)
{
    int i = 0;
    
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
            uart->error = UART_EDATA;
            _uart_error(uart, __func__, "invalid/unsupported data bits");
            return -1;
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
            uart->error = UART_EPARITY;
            _uart_error(uart, __func__, "invalid/unsupported parity");
            return -1;
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
            uart->error = UART_ESTOP;
            _uart_error(uart, __func__, "invalid/unsupported stop bits");
            return -1;
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
            uart->error = UART_EFLOW;
            _uart_error(uart, __func__, "invalid/unsupported flow control");
            return -1;
        }
        
        i++;
        
        if (opt[i] != '\0') {
            uart->error = UART_EOPT;
            _uart_error(uart, __func__, "invalid options");
            return -1;
        }
    }
    
    return UART_ESUCCESS;
}

int UART_init(void)
{
    int ret;

    ret = _uart_init();

    if (ret != UART_ESUCCESS) {
        return ret;
    }

    uart_init_done = 1;

    return UART_ESUCCESS;
}

uart_t *UART_open_dev(const char *dev, enum e_baud baud, const char *opt)
{
    uart_t *p;
    int ret;
    
    p = (uart_t *) malloc(sizeof(uart_t));
    
    if (!p) {
        _uart_error(NULL, __func__, "malloc() failed");
        return NULL;
    }
    
    if (strlen(dev) >= DEV_NAME_LEN) {
        p->error = UART_EDEV;
        _uart_error(p, __func__, "UART device name too long");
        free(p);
        return NULL;
    }
    
    strcpy(p->dev, dev);
    
    if (parse_option(p, opt) != UART_ESUCCESS) {
        free(p);
        return NULL;
    }
    
#ifdef __unix__
    if (!_uart_baud_valid(baud)) {
        free(p);
        return NULL;
    }
#endif

    p->baud = baud;
    
    if (_uart_open(p) == -1) {
        free(p);
        return NULL;
    }

#ifdef LIBUART_THREADS
    p->rx_buffer = buffer_create(DEV_BUFFER_SIZE);

    if (!p->rx_buffer) {
        return NULL;
    }

    p->tx_buffer = buffer_create(DEV_BUFFER_SIZE);

    if (!p->tx_buffer) {
        return NULL;
    }

    ret = _uart_thread_init(p);

    if (ret != UART_ESUCCESS) {
        return NULL;
    }

    ret = _uart_thread_start(p);

    if (ret != UART_ESUCCESS) {
        return NULL;
    }
#endif
    
    return p;
}

ssize_t UART_get_device_list(size_t len, uart_t **ret_uarts)
{
    return 0;
}

int UART_open(uart_t *uart, enum e_baud baud, const char *opt)
{
    return 0;
}

void UART_free(uart_t *uart)
{
    if (!uart)
        return;

    _uart_flush(uart);
    _uart_close(uart);
    free(uart);
}

void UART_free_device_list(uart_t **uarts, size_t len)
{
    size_t i;

    if (!uarts)
        return;

    for (i = 0; i < len; i++) {
        if (uarts[i]) {
            _uart_flush(uarts[i]);
            _uart_close(uarts[i]);
            free(uarts[i]);
        }
    }
}

void UART_close(uart_t *uart)
{
    if (!uart)
        return;

    _uart_flush(uart);
    _uart_close(uart);
}

ssize_t UART_send(uart_t *uart, void *send_buf, size_t len)
{
    if (!uart) {
        _uart_error(NULL, __func__, "invalid UART object");
        return UART_EHANDLE;
    }
    
    if (!send_buf) {
        uart->error = UART_EINVAL;
        _uart_error(NULL, __func__, "invalid send buffer pointer");

        return UART_EINVAL;
    }
    
    if (len < 1) {
        uart->error = UART_EINVAL;
        _uart_error(NULL, __func__, "invalid send buffer length");

        return UART_EINVAL;
    }

#ifndef LIBUART_THREADS
    return _uart_send(uart, send_buf, len);
#else
    _uart_thread_lock_tx(uart);

    if (buffer_get_free(uart->tx_buffer) >= (ssize_t) len) {
        buffer_wr(uart->tx_buffer, send_buf, (ssize_t) len);
    }

    _uart_thread_unlock_tx(uart);

    return (ssize_t) len;
#endif
}

ssize_t UART_recv(uart_t *uart, void *recv_buf, size_t len)
{
    if (!uart) {
        _uart_error(NULL, __func__, "invalid UART object");
        return UART_EHANDLE;
    }
    
    if (!recv_buf) {
        uart->error = UART_EINVAL;
        _uart_error(NULL, __func__, "invalid receive buffer pointer");

        return UART_EINVAL;
    }
    
    if (len < 1) {
        uart->error = UART_EINVAL;
        _uart_error(NULL, __func__, "invalid receive buffer length");

        return UART_EINVAL;
    }

#ifndef LIBUART_THREADS
    return _uart_recv(uart, recv_buf, len);
#else
    _uart_thread_lock_rx(uart);

    if (buffer_get_num(uart->tx_buffer) >= (ssize_t) len) {
        buffer_rd(uart->rx_buffer, recv_buf, (ssize_t) len);
    }

    _uart_thread_unlock_rx(uart);

    return (ssize_t) len;
#endif
}

ssize_t UART_puts(uart_t *uart, char *msg)
{
    if (!uart) {
        _uart_error(NULL, __func__, "invalid UART object");
        return UART_EHANDLE;
    }
    
    if (!msg) {
        uart->error = UART_EINVAL;
        _uart_error(NULL, __func__, "invalid string");

        return UART_EINVAL;
    }
    
    return _uart_send(uart, msg, strlen(msg));
}

int UART_putc(uart_t *uart, char c)
{
    if (!uart) {
        _uart_error(NULL, __func__, "invalid UART object");
        return UART_EHANDLE;
    }

    return _uart_send(uart, &c, 1);
}

int UART_getc(uart_t *uart, char *ret_c)
{
    char buf[1];
    int ret;
    
    if (!uart) {
        _uart_error(NULL, __func__, "invalid UART object");
        return UART_EHANDLE;
    }
    
    if (!ret_c) {
        uart->error = UART_EINVAL;
        _uart_error(NULL, __func__, "invalid char pointer");
        return -1;
    }
    
    ret = _uart_recv(uart, &buf[0], 1);
    (*ret_c) = buf[0];

    return ret;
}

int UART_flush(uart_t *uart)
{
    if (!uart) {
        _uart_error(NULL, __func__, "invalid UART object");
        return UART_EHANDLE;
    }

#ifndef LIBUART_THREADS
    return _uart_flush(uart);
#else

#endif
}

int UART_set_baud(uart_t *uart, enum e_baud baud)
{
    if (!uart) {
        _uart_error(NULL, __func__, "invalid UART object");
        return UART_EHANDLE;
    }
    
    uart->baud = baud;
    return _uart_init_baud(uart);
}

int UART_get_baud(uart_t *uart, int *ret_baud)
{
    if (!uart) {
        _uart_error(NULL, __func__, "invalid UART object");
        return UART_EHANDLE;
    }
    
    if (!ret_baud) {
        uart->error = UART_EINVAL;
        _uart_error(NULL, __func__, "invalid int pointer");

        return UART_EINVAL;
    }
    
    (*ret_baud) = uart->baud;

    return UART_ESUCCESS;
}

#ifdef __unix__
int UART_get_fd(uart_t *uart, int *ret_fd)
{
    if (!uart) {
        _uart_error(NULL, __func__, "invalid UART object");
        return UART_EHANDLE;
    }
    
    if (!ret_fd) {
        uart->error = UART_EINVAL;
        _uart_error(NULL, __func__, "invalid int pointer");

        return UART_EINVAL;
    }
    
    (*ret_fd) = uart->fd;

    return UART_ESUCCESS;
}
#elif _WIN32
int UART_get_handle(uart_t *uart, HANDLE *ret_h)
{
    if (!uart) {
        _uart_error(NULL, __func__, "invalid UART object");
        return UART_EHANDLE;
    }

    if (!ret_h) {
        uart->error = UART_EINVAL;
        _uart_error(NULL, __func__, "invalid HANDLE");

        return UART_EINVAL;
    }

    (*ret_h) = uart->h;

    return UART_ESUCCESS;
}
#endif

int UART_get_dev(uart_t *uart, char **ret_dev)
{
    if (!uart) {
        _uart_error(NULL, __func__, "invalid UART object");
        return UART_EHANDLE;
    }
    
    if (!ret_dev) {
        uart->error = UART_EINVAL;
        _uart_error(NULL, __func__, "invalid char pointer");

        return UART_EINVAL;
    }
    
    (*ret_dev) = uart->dev;

    return UART_ESUCCESS;
}

int UART_set_databits(uart_t *uart, enum e_data data_bits)
{
    if (!uart) {
        _uart_error(NULL, __func__, "invalid UART object");
        return UART_EHANDLE;
    }
    
    uart->data_bits = data_bits;

    return _uart_init_databits(uart);
}

int UART_get_databits(uart_t *uart, int *ret_data_bits)
{
    if (!uart) {
        _uart_error(NULL, __func__, "invalid UART object");
        return UART_EHANDLE;
    }
    
    if (!ret_data_bits) {
        uart->error = UART_EINVAL;
        _uart_error(NULL, __func__, "invalid int pointer");

        return UART_EINVAL;
    }
    
    (*ret_data_bits) = uart->data_bits;

    return UART_ESUCCESS;
}

int UART_set_parity(uart_t *uart, enum e_parity parity)
{
    if (!uart) {
        _uart_error(NULL, __func__, "invalid UART object");
        return UART_EHANDLE;
    }
    
    uart->parity = parity;

    return _uart_init_parity(uart);
}

int UART_get_parity(uart_t *uart, int *ret_parity)
{
    if (!uart) {
        _uart_error(NULL, __func__, "invalid UART object");
        return UART_EHANDLE;
    }
    
    if (!ret_parity) {
        uart->error = UART_EINVAL;
        _uart_error(NULL, __func__, "invalid int pointer");

        return UART_EINVAL;
    }
    
    (*ret_parity) = uart->parity;

    return UART_ESUCCESS;
}

int UART_set_stopbits(uart_t *uart, enum e_stop stop_bits)
{
    if (!uart) {
        _uart_error(NULL, __func__, "invalid UART object");
        return UART_EHANDLE;
    }
    
    uart->stop_bits = stop_bits;

    return _uart_init_stopbits(uart);
}

int UART_get_stopbits(uart_t *uart, int *ret_stop_bits)
{
    if (!uart) {
        _uart_error(NULL, __func__, "invalid UART object");
        return UART_EHANDLE;
    }
    
    if (!ret_stop_bits) {
        uart->error = UART_EINVAL;
        _uart_error(NULL, __func__, "invalid int pointer");

        return UART_EINVAL;
    }
    
    (*ret_stop_bits) = uart->stop_bits;

    return UART_ESUCCESS;
}

int UART_set_flowctrl(uart_t *uart, enum e_flow flow_ctrl)
{
    if (!uart) {
        _uart_error(NULL, __func__, "invalid UART object");
        return UART_EHANDLE;
    }
    
    uart->flow_ctrl = flow_ctrl;

    return _uart_init_flow(uart);
}

int UART_get_flowctrl(uart_t *uart, int *ret_flow_ctrl)
{
    if (!uart) {
        _uart_error(NULL, __func__, "invalid UART object");
        return UART_EHANDLE;
    }
    
    if (!ret_flow_ctrl) {
        uart->error = UART_EINVAL;
        _uart_error(NULL, __func__, "invalid int pointer");

        return UART_EINVAL;
    }
    
    (*ret_flow_ctrl) = uart->flow_ctrl;

    return UART_ESUCCESS;
}

int UART_set_pin(uart_t *uart, enum e_pins pin, int state)
{
    if (!uart) {
        _uart_error(NULL, __func__, "invalid UART object");
        return UART_EHANDLE;
    }
    
    return _uart_set_pin(uart, pin, state);
}

int UART_get_pin(uart_t *uart, enum e_pins pin, int *ret_state)
{
    if (!uart) {
        _uart_error(NULL, __func__, "invalid UART object");
        return UART_EHANDLE;
    }
    
    if (!ret_state) {
        uart->error = UART_EINVAL;
        _uart_error(NULL, __func__, "invalid int pointer");

        return UART_EINVAL;
    }
    
    return _uart_get_pin(uart, pin, ret_state);
}

int UART_get_bytes_available(uart_t *uart, int *ret_num)
{
    if (!uart) {
        _uart_error(NULL, __func__, "invalid UART object");
        return UART_EHANDLE;
    }
    
    if (!ret_num) {
        uart->error = UART_EINVAL;
        _uart_error(NULL, __func__, "invalid int pointer");

        return UART_EINVAL;
    }
    
    return _uart_get_bytes(uart, ret_num);
}

int UART_get_errno(uart_t *uart)
{

    if (!uart) {
        _uart_error(NULL, __func__, "invalid UART object");

        return UART_EHANDLE;
    }

    return uart->error;
}

char *UART_get_errmsg(uart_t *uart)
{
    if (!uart) {
        _uart_error(NULL, __func__, "invalid UART object");

        return NULL;
    }

    return uart->err_msg;
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
