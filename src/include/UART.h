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

#ifdef _WIN32
#include <windows.h>
#ifdef LIBUART_EXPORTS
#define LIBUART_API __declspec(dllexport)
#else
#define LIBUART_API __declspec(dllimport)
#endif
#else
#include <sys/types.h>
#endif

/**
 * libUART error codes
 */
#define UART_ESUCCESS       0       /* No error (success) */
#define UART_EINVAL         (-1)    /* Invalid argument */
#define UART_ENOMEM         (-2)    /* No free memory */
#define UART_ESYSAPI        (-3)    /* System API call error */
#define UART_EOPT           (-4)    /* Invalid option */
#define UART_EDEV           (-5)    /* Invalid device */
#define UART_EBAUD          (-6)    /* Invalid baud rate */
#define UART_EDATA          (-7)    /* Invalid data bits */
#define UART_EPARITY        (-8)    /* Invalid parity */
#define UART_ESTOP          (-9)    /* Invalid stop bits */
#define UART_EFLOW          (-10)   /* Invalid flow control */
#define UART_EPIN           (-11)   /* Invalid pin */
#define UART_EPERM          (-12)   /* Access permission */
#define UART_EHANDLE        (-13)   /* Invalid UART object/handle */
#define UART_ECTX           (-14)   /* Invalid context */
#define UART_EBUF           (-15)   /* Buffer full or empty (only with threading support) */

struct _uart_ctx;
typedef struct _uart_ctx uart_ctx_t;

struct _uart;
typedef struct _uart uart_t;

/**
 * UART (default) baud rates
 */
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
#ifdef __linux__
    UART_BAUD_576000 = 576000,
#endif
    UART_BAUD_921600 = 921600,
    UART_BAUD_1000000 = 1000000,
#ifdef __linux__
    UART_BAUD_1152000 = 1152000,
#endif
    UART_BAUD_1500000 = 1500000,
    UART_BAUD_2000000 = 2000000,
    UART_BAUD_2500000 = 2500000,
    UART_BAUD_3000000 = 3000000,
    UART_BAUD_3500000 = 3500000,
    UART_BAUD_4000000 = 4000000
#endif
};

/**
 * UART data bits length
 */
enum e_data {
    UART_DATA_5 = 5,
    UART_DATA_6 = 6,
    UART_DATA_7 = 7,
    UART_DATA_8 = 8,
    UART_DATA_16 = 16       /* Currently not supported */
};

/**
 * UART parity
 */
enum e_parity {
    UART_PARITY_NONE,       /* None */
    UART_PARITY_ODD,        /* Odd parity */
    UART_PARITY_EVEN        /* Even parity */
};

/**
 * UART stop bits length
 */
enum e_stop {
    UART_STOP_1_0,
    UART_STOP_1_5,          /* Currently not supported */
    UART_STOP_2_0
};

/**
 * UART flow control
 */
enum e_flow {
    UART_FLOW_NO,           /* None */
    UART_FLOW_SW,           /* Software */
    UART_FLOW_HW            /* Hardware */
};

/**
 * UART readable/writable pins
 */
enum e_pins {
    UART_PIN_RTS,           /* Request to Send (out) */
    UART_PIN_CTS,           /* Clear to Send (in) */
    UART_PIN_DSR,           /* Data Set Ready (in) */
    UART_PIN_DCD,           /* Data Carrier Detect (in) */
    UART_PIN_DTR,           /* Data Terminal Ready (out) */
    UART_PIN_RI             /* Ring Indicator (in) */
};

/* UART pin states */
#define UART_PIN_LOW        0
#define UART_PIN_HIGH       1

#ifdef __unix__
/**
 * libUART Basic Functions
 */

/* Create library context and initialize */
extern int UART_init(uart_ctx_t **ret_ctx);

/* Free library context */
extern int UART_free(uart_ctx_t *ctx);

/* Return a list from all current available UART devices on system */
extern ssize_t UART_get_device_list(uart_ctx_t *ctx, uart_t **ret_uarts);

/* Opens an UART interface by device name */
extern uart_t *UART_dev_open_name(uart_ctx_t *ctx, const char *devname, enum e_baud baud, const char *opt);

/* Opens an UART interface */
extern int UART_dev_open(uart_ctx_t *ctx, uart_t *uart, enum e_baud baud, const char *opt);

/* Closes the UART interface */
extern int UART_dev_close(uart_ctx_t *ctx, uart_t *uart);

/* Free UART device */
extern int UART_dev_free(uart_ctx_t *ctx, uart_t *uart);

/**
 * libUART Basic Input/Output Functions
 */

/* Send data over the UART interface */
extern ssize_t UART_send(uart_ctx_t *ctx, uart_t *uart, void *send_buf, size_t len);

/* Receive data from the UART interface */
extern ssize_t UART_recv(uart_ctx_t *ctx, uart_t *uart, void *recv_buf, size_t len);

/**
 * libUART Input/Output Functions
 */

/* Send a string over the UART interface */
extern ssize_t UART_puts(uart_ctx_t *ctx, uart_t *uart, char *msg);

/* Send a character over the UART interface */
extern int UART_putc(uart_ctx_t *ctx, uart_t *uart, char c);

/* Receive a character from the UART interface */
extern int UART_getc(uart_ctx_t *ctx, uart_t *uart, char *ret_c);

/* Flush not sent data from the UART interface */
extern int UART_flush(uart_ctx_t *ctx, uart_t *uart);

/* Set pin state from the UART interface */
extern int UART_set_pin(uart_ctx_t *ctx, uart_t *uart, enum e_pins pin, int state);

/* Get pin state from the UART interface */
extern int UART_get_pin(uart_ctx_t *ctx, uart_t *uart, enum e_pins pin, int *ret_state);

/**
 * libUART Configuration Functions
 */

/* Set baud rate from the UART interface */
extern int UART_set_baud(uart_ctx_t *ctx, uart_t *uart, enum e_baud baud);

/* Get baud rate from the UART interface */
extern int UART_get_baud(uart_ctx_t *ctx, uart_t *uart, int *ret_baud);

/* Set number of data bits from the UART interface */
extern int UART_set_databits(uart_ctx_t *ctx, uart_t *uart, enum e_data data_bits);

/* Get number of data bits from the UART interface */
extern int UART_get_databits(uart_ctx_t *ctx, uart_t *uart, int *ret_data_bits);

/* Set parity from the UART interface */
extern int UART_set_parity(uart_ctx_t *ctx, uart_t *uart, enum e_parity parity);

/* Get parity from the UART interface */
extern int UART_get_parity(uart_ctx_t *ctx, uart_t *uart, int *ret_parity);

/* Set number of stop bits from the UART interface */
extern int UART_set_stopbits(uart_ctx_t *ctx, uart_t *uart, enum e_stop stop_bits);

/* Get number of stop bits from the UART interface */
extern int UART_get_stopbits(uart_ctx_t *ctx, uart_t *uart, int *ret_stop_bits);

/* Set flow control from the UART interface */
extern int UART_set_flowctrl(uart_ctx_t *ctx, uart_t *uart, enum e_flow flow_ctrl);

/* Get flow control from the UART interface */
extern int UART_get_flowctrl(uart_ctx_t *ctx, uart_t *uart, int *ret_flow_ctrl);

/* Get the underlying file descriptor from the UART interface */
extern int UART_get_fd(uart_ctx_t *ctx, uart_t *uart, int *ret_fd);

/* Get the device name from the UART interface */
extern int UART_get_dev(uart_ctx_t *ctx, uart_t *uart, char **ret_dev);

/**
 * libUART Miscellaneous Functions
 */

/* Get the available bytes in the receive channel from the UART interface */
extern int UART_get_bytes_available(uart_ctx_t *ctx, uart_t *uart, int *ret_num);

/* Get last context error number */
extern int UART_get_ctxerrro(uart_ctx_t *ctx);

/* Get last context error message */
extern char *UART_get_ctxerrormsg(uart_ctx_t *ctx);

/* Get last UART device error number */
extern int UART_get_deverrro(uart_ctx_t *ctx, uart_t *uart);

/* Get last UART device error message */
extern char *UART_get_deverrormsg(uart_ctx_t *ctx, uart_t *uart);

/* Get the library name string */
extern char *UART_get_libname(void);

/* Get the library version string */
extern char *UART_get_libversion(void);

#elif _WIN32
/**
 * libUART Basic Functions
 */

/* Create library context and initialize */
extern LIBUART_API int UART_init(uart_ctx_t **ret_ctx);

/* Free library context */
extern LIBUART_API int UART_free(uart_ctx_t *ctx);

/* Return a list from all current available UART devices on system */
extern LIBUART_API ssize_t UART_get_device_list(uart_ctx_t *ctx, uart_t **ret_uarts);

/* Opens an UART interface by device name */
extern LIBUART_API uart_t *UART_dev_open_name(uart_ctx_t *ctx, const char *devname, enum e_baud baud, const char *opt);

/* Opens an UART interface */
extern LIBUART_API int UART_dev_open(uart_ctx_t *ctx, uart_t *uart, enum e_baud baud, const char *opt);

/* Closes the UART interface */
extern LIBUART_API int UART_dev_close(uart_ctx_t *ctx, uart_t *uart);

/* Free UART device */
extern LIBUART_API int UART_dev_free(uart_ctx_t *ctx, uart_t *uart);

/**
 * libUART Basic Input/Output Functions
 */

/* Send data over the UART interface */
extern LIBUART_API ssize_t UART_send(uart_ctx_t *ctx, uart_t *uart, void *send_buf, size_t len);

/* Receive data from the UART interface */
extern LIBUART_API ssize_t UART_recv(uart_ctx_t *ctx, uart_t *uart, void *recv_buf, size_t len);

/**
 * libUART Input/Output Functions
 */

/* Send a string over the UART interface */
extern LIBUART_API ssize_t UART_puts(uart_ctx_t *ctx, uart_t *uart, char *msg);

/* Send a character over the UART interface */
extern LIBUART_API int UART_putc(uart_ctx_t *ctx, uart_t *uart, char c);

/* Receive a character from the UART interface */
extern LIBUART_API int UART_getc(uart_ctx_t *ctx, uart_t *uart, char *ret_c);

/* Flush not sent data from the UART interface */
extern LIBUART_API int UART_flush(uart_ctx_t *ctx, uart_t *uart);

/* Set pin state from the UART interface */
extern LIBUART_API int UART_set_pin(uart_ctx_t *ctx, uart_t *uart, enum e_pins pin, int state);

/* Get pin state from the UART interface */
extern LIBUART_API int UART_get_pin(uart_ctx_t *ctx, uart_t *uart, enum e_pins pin, int *ret_state);

/**
 * libUART Configuration Functions
 */

/* Set baud rate from the UART interface */
extern LIBUART_API int UART_set_baud(uart_ctx_t *ctx, uart_t *uart, enum e_baud baud);

/* Get baud rate from the UART interface */
extern LIBUART_API int UART_get_baud(uart_ctx_t *ctx, uart_t *uart, int *ret_baud);

/* Set number of data bits from the UART interface */
extern LIBUART_API int UART_set_databits(uart_ctx_t *ctx, uart_t *uart, enum e_data data_bits);

/* Get number of data bits from the UART interface */
extern LIBUART_API int UART_get_databits(uart_ctx_t *ctx, uart_t *uart, int *ret_data_bits);

/* Set parity from the UART interface */
extern LIBUART_API int UART_set_parity(uart_ctx_t *ctx, uart_t *uart, enum e_parity parity);

/* Get parity from the UART interface */
extern LIBUART_API int UART_get_parity(uart_ctx_t *ctx, uart_t *uart, int *ret_parity);

/* Set number of stop bits from the UART interface */
extern LIBUART_API int UART_set_stopbits(uart_ctx_t *ctx, uart_t *uart, enum e_stop stop_bits);

/* Get number of stop bits from the UART interface */
extern LIBUART_API int UART_get_stopbits(uart_ctx_t *ctx, uart_t *uart, int *ret_stop_bits);

/* Set flow control from the UART interface */
extern LIBUART_API int UART_set_flowctrl(uart_ctx_t *ctx, uart_t *uart, enum e_flow flow_ctrl);

/* Get flow control from the UART interface */
extern LIBUART_API int UART_get_flowctrl(uart_ctx_t *ctx, uart_t *uart, int *ret_flow_ctrl);

/* Get the underlying file handle from the UART interface */
extern LIBUART_API int UART_get_handle(uart_ctx_t *ctx, uart_t *uart, HANDLE *ret_h);

/* Get the device name from the UART interface */
extern LIBUART_API int UART_get_dev(uart_ctx_t *ctx, uart_t *uart, char **ret_dev);

/**
 * libUART Miscellaneous Functions
 */

/* Get the available bytes in the receive channel from the UART interface */
extern LIBUART_API int UART_get_bytes_available(uart_ctx_t *ctx, uart_t *uart, int *ret_num);

/* Get last context error number */
extern LIBUART_API int UART_get_ctxerrro(uart_ctx_t *ctx);

/* Get last context error message */
extern LIBUART_API char *UART_get_ctxerrormsg(uart_ctx_t *ctx);

/* Get last UART device error number */
extern LIBUART_API int UART_get_deverrro(uart_ctx_t *ctx, uart_t *uart);

/* Get last UART device error message */
extern LIBUART_API char *UART_get_deverrormsg(uart_ctx_t *ctx, uart_t *uart);

/* Get the library name string */
extern LIBUART_API char *UART_get_libname(void);

/* Get the library version string */
extern LIBUART_API char *UART_get_libversion(void);

#endif

#endif
