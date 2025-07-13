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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "error.h"
#include "version.h"

#include <UART.h>

#define UART_EMSGLEN        256

void _uart_error(struct _uart *uart, const char *func, const char *err_msg)
{
    char buf[UART_EMSGLEN];
    
    if (uart->err_msg_len != 0) {
        uart->err_msg = (char *) malloc(UART_EMSGLEN);
        
        if (!uart->err_msg) {
            uart->error = UART_ENOMEM;
            return;
        }
        
        uart->err_msg_len = UART_EMSGLEN;
    }
    
    if (func)
        sprintf(uart->err_msg, "[%s] error: %s(): ", LIBUART_NAME, func);
    else
        sprintf(uart->err_msg, "[%s] error: ", LIBUART_NAME);
    
    
    if (err_msg) {
        if (errno != 0)
            sprintf(buf, "%s (%s)\n", err_msg, strerror(errno));
        else
            sprintf(buf, "%s\n", err_msg);
    } else
        sprintf(buf, "\n");
    
    if ((strlen(uart->err_msg) + strlen(buf)) < (size_t) uart->err_msg_len)
        strcat(uart->err_msg, buf);
    else {
        uart->err_msg = (char *) realloc(uart->err_msg, uart->err_msg_len + UART_EMSGLEN);
        
        if (!uart->err_msg) {
            uart->error = UART_ENOMEM;
            return;
        }
        
        uart->err_msg_len += UART_EMSGLEN;
        strcat(uart->err_msg, buf);
    }
}
