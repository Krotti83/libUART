/**
 *
 * libUART
 *
 * Easy to use library for accessing the UART
 *
 * Copyright (c) 2025, 2026 Johannes Krottmayer <krotti83@proton.me>
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

#ifndef LIBUART_UART_H
#define LIBUART_UART_H  1
#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>
#include <stdarg.h>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
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
#define UART_EBUF           (-15)   /* Buffer full or empty, or too small */

struct _uart_ctx;
typedef struct _uart_ctx UART_ctx;

struct _uart_dev;
typedef struct _uart_dev UART_dev;

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
#ifdef _WIN32
    UART_DATA_16 = 16       /* Windows only (experimental) */
#endif
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
    UART_STOP_1_0,          /* 1 stop bit */
#ifdef _WIN32
    UART_STOP_1_5,          /* 1.5 stop bits - Windows only (experimental) */
#endif
    UART_STOP_2_0           /* 2 stop bits */
};

/**
 * UART flow control
 */
enum e_flow {
    UART_FLOW_NONE,         /* No flow control */
    UART_FLOW_SOFTWARE,     /* Software flow control */
    UART_FLOW_HARDWARE      /* Hardware flow control */
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

/**
 * UART pin states
 */
enum e_pin_state {
    UART_PIN_LOW,
    UART_PIN_HIGH
};

#ifdef __unix__
/**
 * libUART Basic Functions
 */

/* Create library context and initialize */
extern int UART_init(UART_ctx **ret_ctx);

/* Free library context */
extern int UART_free(UART_ctx *ctx);

/* Return a list from all current available UART devices on system */
extern ssize_t UART_get_device_list(UART_ctx *ctx,
                                    UART_dev **ret_uarts,
                                    size_t *ret_num);

/* Opens an UART interface by device name */
extern UART_dev *UART_dev_open_name(UART_ctx *ctx,
                                    const char *devname,
                                    enum e_baud baud,
                                    const char *opt);

/* Opens an UART interface */
extern int UART_dev_open(UART_ctx *ctx,
                         UART_dev *uart,
                         enum e_baud baud,
                         const char *opt);

/* Closes the UART interface */
extern int UART_dev_close(UART_ctx *ctx,
                          UART_dev *uart);

/* Frees the UART device */
extern int UART_dev_free(UART_ctx *ctx,
                         UART_dev *uart);

/**
 * libUART Basic Input/Output Functions
 */

/* Send data over the UART interface */
extern ssize_t UART_send(UART_ctx *ctx,
                         UART_dev *uart,
                         const void *send_buf,
                         size_t len);

/* Receive data from the UART interface */
extern ssize_t UART_recv(UART_ctx *ctx,
                         UART_dev *uart,
                         void *recv_buf,
                         size_t len);

/**
 * libUART Input/Output Functions
 */

/* Send a string over the UART interface */
extern ssize_t UART_puts(UART_ctx *ctx,
                         UART_dev *uart,
                         const char *msg);

/* Send a formatted string over the UART interface */
extern ssize_t UART_printf(UART_ctx *ctx,
                           UART_dev *uart,
                           const char *fmt, ...);

/* Send a character over the UART interface */
extern int UART_putc(UART_ctx *ctx,
                     UART_dev *uart,
                     const char c);

/* Receive a character from the UART interface */
extern int UART_getc(UART_ctx *ctx,
                     UART_dev *uart,
                     char *ret_c);

/* Flush not sent data from the UART interface */
extern int UART_flush(UART_ctx *ctx,
                      UART_dev *uart);

/* Set pin state from the UART interface */
extern int UART_set_pin(UART_ctx *ctx,
                        UART_dev *uart,
                        enum e_pins pin,
                        enum e_pin_state state);

/* Get pin state from the UART interface */
extern int UART_get_pin(UART_ctx *ctx,
                        UART_dev *uart,
                        enum e_pins pin,
                        int *ret_state);

/**
 * libUART Configuration Functions
 */

/* Set baud rate from the UART interface */
extern int UART_set_baud(UART_ctx *ctx,
                         UART_dev *uart,
                         enum e_baud baud);

/* Get baud rate from the UART interface */
extern int UART_get_baud(UART_ctx *ctx,
                         UART_dev *uart,
                         int *ret_baud);

/* Set number of data bits from the UART interface */
extern int UART_set_databits(UART_ctx *ctx,
                             UART_dev *uart,
                             enum e_data data_bits);

/* Get number of data bits from the UART interface */
extern int UART_get_databits(UART_ctx *ctx,
                             UART_dev *uart,
                             int *ret_data_bits);

/* Set parity from the UART interface */
extern int UART_set_parity(UART_ctx *ctx,
                           UART_dev *uart,
                           enum e_parity parity);

/* Get parity from the UART interface */
extern int UART_get_parity(UART_ctx *ctx,
                           UART_dev *uart,
                           int *ret_parity);

/* Set number of stop bits from the UART interface */
extern int UART_set_stopbits(UART_ctx *ctx,
                             UART_dev *uart,
                             enum e_stop stop_bits);

/* Get number of stop bits from the UART interface */
extern int UART_get_stopbits(UART_ctx *ctx,
                             UART_dev *uart,
                             int *ret_stop_bits);

/* Set flow control from the UART interface */
extern int UART_set_flowctrl(UART_ctx *ctx,
                             UART_dev *uart,
                             enum e_flow flow_ctrl);

/* Get flow control from the UART interface */
extern int UART_get_flowctrl(UART_ctx *ctx,
                             UART_dev *uart,
                             int *ret_flow_ctrl);

/* Get the underlying file descriptor from the UART interface */
extern int UART_get_fd(UART_ctx *ctx,
                       UART_dev *uart,
                       int *ret_fd);

/* Get the device name from the UART interface */
extern int UART_get_dev(UART_ctx *ctx,
                        UART_dev *uart,
                        char **ret_dev);

/**
 * libUART Miscellaneous Functions
 */

/* Get the available bytes in the receive channel from the UART interface */
extern int UART_get_bytes_available(UART_ctx *ctx,
                                    UART_dev *uart,
                                    size_t *ret_num);

/* Get last context error number */
extern int UART_get_ctxerror(UART_ctx *ctx);

/* Get last context error message */
extern char *UART_get_ctxerrormsg(UART_ctx *ctx);

/* Clear context error */
extern int UART_clear_ctxerror(UART_ctx *ctx);

/* Get last UART device error number */
extern int UART_get_deverror(UART_ctx *ctx,
                             UART_dev *uart);

/* Clear UART device error */
extern int UART_clear_deverror(UART_ctx *ctx,
                               UART_dev *uart);

/* Get last UART device error message */
extern char *UART_get_deverrormsg(UART_ctx *ctx,
                                  UART_dev *uart);

/* Get the library name string */
extern char *UART_get_libname(void);

/* Get the library version string */
extern char *UART_get_libversion(void);

#elif _WIN32
/**
 * libUART Basic Functions
 */

/* Create library context and initialize */
extern LIBUART_API int UART_init(UART_ctx **ret_ctx);

/* Free library context */
extern LIBUART_API int UART_free(UART_ctx *ctx);

/* Return a list from all current available UART devices on system */
extern LIBUART_API ssize_t UART_get_device_list(UART_ctx *ctx,
                                                UART_dev **ret_uarts,
                                                size_t *ret_num);

/* Opens an UART interface by device name */
extern LIBUART_API UART_dev *UART_dev_open_name(UART_ctx *ctx,
                                                const char *devname,
                                                enum e_baud baud,
                                                const char *opt);

/* Opens an UART interface */
extern LIBUART_API int UART_dev_open(UART_ctx *ctx,
                                     UART_dev *uart,
                                     enum e_baud baud,
                                     const char *opt);

/* Closes the UART interface */
extern LIBUART_API int UART_dev_close(UART_ctx *ctx,
                                      UART_dev *uart);

/* Free UART device */
extern LIBUART_API int UART_dev_free(UART_ctx *ctx,
                                     UART_dev *uart);

/**
 * libUART Basic Input/Output Functions
 */

/* Send data over the UART interface */
extern LIBUART_API ssize_t UART_send(UART_ctx *ctx,
                                     UART_dev *uart,
                                     const void *send_buf,
                                     size_t len);

/* Receive data from the UART interface */
extern LIBUART_API ssize_t UART_recv(UART_ctx *ctx,
                                     UART_dev *uart,
                                     void *recv_buf,
                                     size_t len);

/**
 * libUART Input/Output Functions
 */

/* Send a string over the UART interface */
extern LIBUART_API ssize_t UART_puts(UART_ctx *ctx,
                                     UART_dev *uart,
                                     const char *msg);

/* Send a formatted string over the UART interface */
extern LIBUART_API ssize_t UART_printf(UART_ctx *ctx,
                                       UART_dev *uart,
                                       const char *fmt,
                                       ...);

/* Send a character over the UART interface */
extern LIBUART_API int UART_putc(UART_ctx *ctx,
                                 UART_dev *uart,
                                 char c);

/* Receive a character from the UART interface */
extern LIBUART_API int UART_getc(UART_ctx *ctx,
                                 UART_dev *uart,
                                 char *ret_c);

/* Flush not sent data from the UART interface */
extern LIBUART_API int UART_flush(UART_ctx *ctx,
                                  UART_dev *uart);

/* Set pin state from the UART interface */
extern LIBUART_API int UART_set_pin(UART_ctx *ctx,
                                    UART_dev *uart,
                                    enum e_pins pin,
                                    enum e_pin_state state);

/* Get pin state from the UART interface */
extern LIBUART_API int UART_get_pin(UART_ctx *ctx,
                                    UART_dev *uart,
                                    enum e_pins pin,
                                    int *ret_state);

/**
 * libUART Configuration Functions
 */

/* Set baud rate from the UART interface */
extern LIBUART_API int UART_set_baud(UART_ctx *ctx,
                                     UART_dev *uart,
                                     enum e_baud baud);

/* Get baud rate from the UART interface */
extern LIBUART_API int UART_get_baud(UART_ctx *ctx,
                                     UART_dev *uart,
                                     int *ret_baud);

/* Set number of data bits from the UART interface */
extern LIBUART_API int UART_set_databits(UART_ctx *ctx,
                                         UART_dev *uart,
                                         enum e_data data_bits);

/* Get number of data bits from the UART interface */
extern LIBUART_API int UART_get_databits(UART_ctx *ctx,
                                         UART_dev *uart,
                                         int *ret_data_bits);

/* Set parity from the UART interface */
extern LIBUART_API int UART_set_parity(UART_ctx *ctx,
                                       UART_dev *uart,
                                       enum e_parity parity);

/* Get parity from the UART interface */
extern LIBUART_API int UART_get_parity(UART_ctx *ctx,
                                       UART_dev *uart,
                                       int *ret_parity);

/* Set number of stop bits from the UART interface */
extern LIBUART_API int UART_set_stopbits(UART_ctx *ctx,
                                         UART_dev *uart,
                                         enum e_stop stop_bits);

/* Get number of stop bits from the UART interface */
extern LIBUART_API int UART_get_stopbits(UART_ctx *ctx,
                                         UART_dev *uart,
                                         int *ret_stop_bits);

/* Set flow control from the UART interface */
extern LIBUART_API int UART_set_flowctrl(UART_ctx *ctx,
                                         UART_dev *uart,
                                         enum e_flow flow_ctrl);

/* Get flow control from the UART interface */
extern LIBUART_API int UART_get_flowctrl(UART_ctx *ctx,
                                         UART_dev *uart,
                                         int *ret_flow_ctrl);

/* Get the underlying file handle from the UART interface */
extern LIBUART_API int UART_get_handle(UART_ctx *ctx,
                                       UART_dev *uart,
                                       HANDLE *ret_h);

/* Get the device name from the UART interface */
extern LIBUART_API int UART_get_dev(UART_ctx *ctx,
                                    UART_dev *uart,
                                    char **ret_dev);

/**
 * libUART Miscellaneous Functions
 */

/* Get the available bytes in the receive channel from the UART interface */
extern LIBUART_API int UART_get_bytes_available(UART_ctx *ctx,
                                                UART_dev *uart,
                                                size_t *ret_num);

/* Get last context error number */
extern LIBUART_API int UART_get_ctxerror(UART_ctx *ctx);

/* Get last context error message */
extern LIBUART_API char *UART_get_ctxerrormsg(UART_ctx *ctx);

/* Clear context error */
extern LIBUART_API int UART_clear_ctxerror(UART_ctx *ctx);

/* Get last UART device error number */
extern LIBUART_API int UART_get_deverror(UART_ctx *ctx,
                                         UART_dev *uart);

/* Get last UART device error message */
extern LIBUART_API char *UART_get_deverrormsg(UART_ctx *ctx,
                                              UART_dev *uart);

/* Clear UART device error */
extern LIBUART_API int UART_clear_deverror(UART_ctx *ctx,
                                           UART_dev *uart);

/* Get the library name string */
extern LIBUART_API char *UART_get_libname(void);

/* Get the library version string */
extern LIBUART_API char *UART_get_libversion(void);

#endif

#ifdef __cplusplus
}
#endif
#endif
