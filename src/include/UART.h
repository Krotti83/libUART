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

#ifndef _LIBUART_UART_H
#define _LIBUART_UART_H

#include <stddef.h>

#ifndef _LIBUART_SSIZE_T
#define _LIBUART_SSIZE_T
typedef long int ssize_t;
#endif

/* UART error codes */
#define UART_ESUCCESS       0   /* no error */
#define UART_EINVAL         1   /* invalid argumet */
#define UART_ENOMEM         2   /* no free memory (malloc) */
#define UART_ESYSTEM        3   /* system error (API call error) */
#define UART_EOPT           4   /* invalid option */
#define UART_EDEV           5   /* invalid device */
#define UART_EBAUD          6   /* invalid baud rate */
#define UART_EDATA          7   /* invalid data bits */
#define UART_EPARITY        8   /* invalid parity */
#define UART_ESTOP          9   /* invalid stop bits */
#define UART_EFLOW          10  /* invalid flow control */
#define UART_EPIN           11  /* invalid pin */

struct _uart;

typedef struct _uart uart_t;

/* UART default baud rates */
enum e_baud {
#ifdef __unix__
    UART_BAUD_0 = 0,
    UART_BAUD_50 = 50,
    UART_BAUD_75 = 75,
#endif
    UART_BAUD_110 = 110,
#ifdef __unix__
    UART_BAUD_134 = 134,
    UART_BAUD_150 = 150,
    UART_BAUD_200 = 200,
#endif
    UART_BAUD_300 = 300,
    UART_BAUD_600 = 600,
    UART_BAUD_1200 = 1200,
#ifdef __unix__
    UART_BAUD_1800 = 1800,
#endif
    UART_BAUD_2400 = 2400,
    UART_BAUD_4800 = 4800,
    UART_BAUD_9600 = 9600,
#ifdef _WIN32
    UART_BAUD_14400 = 14400,
#endif
    UART_BAUD_19200 = 19200,
    UART_BAUD_38400 = 38400,
    UART_BAUD_57600 = 57600,
    UART_BAUD_115200 = 115200,
#ifdef _WIN32
    UART_BAUD_128000 = 128000,
    UART_BAUD_256000 = 256000
#elif __unix__
    UART_BAUD_230400 = 230400,
    UART_BAUD_460800 = 460800,
    UART_BAUD_500000 = 500000,
    UART_BAUD_576000 = 576000,
    UART_BAUD_921600 = 921600,
    UART_BAUD_1000000 = 1000000,
    UART_BAUD_1152000 = 1152000,
    UART_BAUD_1500000 = 1500000,
    UART_BAUD_2000000 = 2000000,
    UART_BAUD_2500000 = 2500000,
    UART_BAUD_3000000 = 3000000,
    UART_BAUD_3500000 = 3500000,
    UART_BAUD_4000000 = 4000000
#endif
};

/* UART data bits length */
enum e_data {
    UART_DATA_5 = 5,
    UART_DATA_6 = 6,
    UART_DATA_7 = 7,
    UART_DATA_8 = 8,
    UART_DATA_16 = 16
};

/* UART parity */
enum e_parity {
    UART_PARITY_NONE,
    UART_PARITY_ODD,
    UART_PARITY_EVEN
};

/* UART stop bits length */
enum e_stop {
    UART_STOP_1_0,
    UART_STOP_1_5,
    UART_STOP_2_0
};

/* UART flow control */
enum e_flow {
    UART_FLOW_NO,
    UART_FLOW_SW,
    UART_FLOW_HW
};

/* UART readable/writable pins */
enum e_pins {
    UART_PIN_RTS,   /* Request to Send (out) */
    UART_PIN_CTS,   /* Clear to Send (in) */
    UART_PIN_DSR,   /* Data Set Ready (in) */
    UART_PIN_DCD,   /* Data Carrier Detect (in) */
    UART_PIN_DTR,   /* Data Terminal Ready (out) */
    UART_PIN_RI     /* Ring Indicator (in) */
};

/* UART pin states */
#define UART_PIN_LOW        0
#define UART_PIN_HIGH       1

extern void UART_init(void);
extern uart_t *UART_open(const char *dev, enum e_baud baud, const char *opt);
extern void UART_close(uart_t *uart);
extern ssize_t UART_send(uart_t *uart, char *send_buf, size_t len);
extern ssize_t UART_recv(uart_t *uart, char *recv_buf, size_t len);
extern ssize_t UART_puts(uart_t *uart, char *msg);
extern int UART_putc(uart_t *uart, char c);
extern int UART_getc(uart_t *uart, char *ret_c);
extern int UART_flush(uart_t *uart);
extern int UART_set_baud(uart_t *uart, enum e_baud baud);
extern int UART_get_baud(uart_t *uart, int *ret_baud);
extern int UART_get_fd(uart_t *uart, int *ret_fd);
extern int UART_get_dev(uart_t *uart, char **ret_dev);
extern int UART_set_databits(uart_t *uart, enum e_data data_bits);
extern int UART_get_databits(uart_t *uart, int *ret_data_bits);
extern int UART_set_parity(uart_t *uart, enum e_parity parity);
extern int UART_get_parity(uart_t *uart, int *ret_parity);
extern int UART_set_stopbits(uart_t *uart, enum e_stop stop_bits);
extern int UART_get_stopbits(uart_t *uart, int *ret_stop_bits);
extern int UART_set_flowctrl(uart_t *uart, enum e_flow flow_ctrl);
extern int UART_get_flowctrl(uart_t *uart, int *ret_flow_ctrl);
extern int UART_set_pin(uart_t *uart, enum e_pins pin, int state);
extern int UART_get_pin(uart_t *uart, enum e_pins pin, int *ret_state);
extern int UART_get_bytes_available(uart_t *uart, int *ret_num);
extern void UART_set_errmsg(int msg_enable);
extern char *UART_get_libname(void);
extern char *UART_get_libversion(void);

#endif
