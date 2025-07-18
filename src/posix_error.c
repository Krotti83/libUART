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
#include <string.h>
#include <errno.h>

#include "_uart.h"
#include "_version.h"

#include <UART.h>

void _uart_error(struct _uart_ctx *ctx,
                 struct _uart *uart,
                 int error,
                 const char *error_func,
                 const char *error_msg)
{
    if (!ctx)
        return;

    if (!uart) {
        ctx->error = error;
        memset(ctx->errormsg, 0, UART_ERRORMAX);
        ctx->flags |= UART_CTXFERROR;

        switch (ctx->error) {
            case UART_ESUCCESS:
                if (error_func) {
                    if (error_msg) {
                        snprintf(ctx->errormsg, UART_ERRORMAX,
                                 "%s: success (%s)",
                                 error_func,
                                 error_msg);
                    } else {
                        snprintf(ctx->errormsg, UART_ERRORMAX,
                                 "%s: success",
                                 error_func);
                    }
                } else {
                    if (error_msg) {
                        snprintf(ctx->errormsg, UART_ERRORMAX,
                                 "success (%s)",
                                 error_msg);
                    } else {
                        snprintf(ctx->errormsg, UART_ERRORMAX,
                                 "success");
                    }
                }

                break;
            case UART_EINVAL:
                if (error_func) {
                    if (error_msg) {
                        snprintf(ctx->errormsg, UART_ERRORMAX,
                                 "%s: invalid argument (%s)",
                                 error_func,
                                 error_msg);
                    } else {
                        snprintf(ctx->errormsg, UART_ERRORMAX,
                                 "%s: invalid argument",
                                 error_func);
                    }
                } else {
                    if (error_msg) {
                        snprintf(ctx->errormsg, UART_ERRORMAX,
                                 "invalid argument (%s)",
                                 error_msg);
                    } else {
                        snprintf(ctx->errormsg, UART_ERRORMAX,
                                 "invalid argument");
                    }
                }

                break;
            case UART_ENOMEM:
                if (error_func) {
                    if (error_msg) {
                        snprintf(ctx->errormsg, UART_ERRORMAX,
                                 "%s: no free memory (%s)",
                                 error_func,
                                 error_msg);
                    } else {
                        snprintf(ctx->errormsg, UART_ERRORMAX,
                                 "%s: no free memory",
                                 error_func);
                    }
                } else {
                    if (error_msg) {
                        snprintf(ctx->errormsg, UART_ERRORMAX,
                                 "no free memory (%s)",
                                 error_msg);
                    } else {
                        snprintf(ctx->errormsg, UART_ERRORMAX,
                                 "no free memory");
                    }
                }

                break;
            case UART_ESYSAPI:
                if (error_func) {
                    if (error_msg) {
                        snprintf(ctx->errormsg, UART_ERRORMAX,
                                 "%s: failed: %s (%s)",
                                 error_func,
                                 error_msg,
                                 strerror(errno));
                    } else {
                        snprintf(ctx->errormsg, UART_ERRORMAX,
                                "%s: failed (%s)",
                                error_func,
                                strerror(errno));
                    }
                } else {
                    if (error_msg) {
                        snprintf(ctx->errormsg, UART_ERRORMAX,
                                 "failed: %s (%s)",
                                 error_msg,
                                 strerror(errno));
                    } else {
                        snprintf(ctx->errormsg, UART_ERRORMAX,
                                 "failed (%s)",
                                 strerror(errno));
                    }
                }

                break;
            case UART_EPERM:
                if (error_func) {
                    if (error_msg) {
                        snprintf(ctx->errormsg, UART_ERRORMAX,
                                 "%s: insufficient permissions (%s)",
                                 error_func,
                                 error_msg);
                    } else {
                        snprintf(ctx->errormsg, UART_ERRORMAX,
                                 "%s: insufficient permissions",
                                 error_func);
                    }
                } else {
                    if (error_msg) {
                        snprintf(ctx->errormsg, UART_ERRORMAX,
                                 "insufficient permissions (%s)",
                                 error_msg);
                    } else {
                        snprintf(ctx->errormsg, UART_ERRORMAX,
                                 "insufficient permissions");
                    }
                }

                break;
            case UART_ECTX:
                if (error_func) {
                    if (error_msg) {
                        snprintf(ctx->errormsg, UART_ERRORMAX,
                                 "%s: invalid context (%s)",
                                 error_func,
                                 error_msg);
                    } else {
                        snprintf(ctx->errormsg, UART_ERRORMAX,
                                 "%s: invalid context",
                                 error_func);
                    }
                } else {
                    if (error_msg) {
                        snprintf(ctx->errormsg, UART_ERRORMAX,
                                 "invalid context (%s)",
                                 error_msg);
                    } else {
                        snprintf(ctx->errormsg, UART_ERRORMAX,
                                 "invalid context");
                    }
                }

                break;
            default:
                if (error_func) {
                    if (error_msg) {
                        snprintf(ctx->errormsg, UART_ERRORMAX,
                                 "%s: unknown error code (%d) (%s)",
                                 error_func,
                                 ctx->error,
                                 error_msg);
                    } else {
                        snprintf(ctx->errormsg, UART_ERRORMAX,
                                 "%s: unknown error code (%d)",
                                 error_func,
                                 ctx->error);
                    }
                } else {
                    if (error_msg) {
                        snprintf(ctx->errormsg, UART_ERRORMAX,
                                 "unknown error code (%d) (%s)",
                                 ctx->error,
                                 error_msg);
                    } else {
                        snprintf(ctx->errormsg, UART_ERRORMAX,
                                 "unknown error code (%d)",
                                 ctx->error);
                    }
                }
        }
    } else {
        uart->error = error;
        memset(uart->errormsg, 0, UART_ERRORMAX);
        uart->flags |= UART_FERROR;

        switch (uart->error) {
            case UART_EINVAL:
                if (error_func) {
                    if (error_msg) {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                 "%s: invalid argument (%s)",
                                 error_func,
                                 error_msg);
                    } else {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                 "%s: invalid argument",
                                 error_func);
                    }
                } else {
                    if (error_msg) {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                 "invalid argument (%s)",
                                 error_msg);
                    } else {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                 "invalid argument");
                    }
                }

                break;
            case UART_ENOMEM:
                if (error_func) {
                    if (error_msg) {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                 "%s: no free memory (%s)",
                                 error_func,
                                 error_msg);
                    } else {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                 "%s: no free memory",
                                 error_func);
                    }
                } else {
                    if (error_msg) {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                 "no free memory (%s)",
                                 error_msg);
                    } else {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                 "no free memory");
                    }
                }

                break;
            case UART_ESYSAPI:
                if (error_func) {
                    if (error_msg) {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                 "%s: failed: %s (%s)",
                                 error_func,
                                 error_msg,
                                 strerror(errno));
                    } else {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                "%s: failed (%s)",
                                error_func,
                                strerror(errno));
                    }
                } else {
                    if (error_msg) {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                 "failed: %s (%s)",
                                 error_msg,
                                 strerror(errno));
                    } else {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                 "failed (%s)",
                                 strerror(errno));
                    }
                }

                break;
            case UART_EOPT:
                if (error_func) {
                    if (error_msg) {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                 "%s: invalid option (%s)",
                                 error_func,
                                 error_msg);
                    } else {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                 "%s: invalid option",
                                 error_func);
                    }
                } else {
                    if (error_msg) {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                 "invalid option (%s)",
                                 error_msg);
                    } else {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                 "invalid option");
                    }
                }

                break;
            case UART_EDEV:
                if (error_func) {
                    if (error_msg) {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                 "%s: invalid device (%s)",
                                 error_func,
                                 error_msg);
                    } else {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                 "%s: invalid device",
                                 error_func);
                    }
                } else {
                    if (error_msg) {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                 "invalid device (%s)",
                                 error_msg);
                    } else {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                 "invalid device");
                    }
                }

                break;
            case UART_EBAUD:
                if (error_func) {
                    if (error_msg) {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                 "%s: invalid baud rate (%s)",
                                 error_func,
                                 error_msg);
                    } else {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                 "%s: invalid baud rate",
                                 error_func);
                    }
                } else {
                    if (error_msg) {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                 "invalid baud rate (%s)",
                                 error_msg);
                    } else {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                 "invalid baud rate");
                    }
                }

                break;
            case UART_EDATA:
                if (error_func) {
                    if (error_msg) {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                 "%s: invalid data bits (%s)",
                                 error_func,
                                 error_msg);
                    } else {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                 "%s: invalid data bits",
                                 error_func);
                    }
                } else {
                    if (error_msg) {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                 "invalid data bits (%s)",
                                 error_msg);
                    } else {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                 "invalid data bits");
                    }
                }

                break;
            case UART_EPARITY:
                if (error_func) {
                    if (error_msg) {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                 "%s: invalid parity (%s)",
                                 error_func,
                                 error_msg);
                    } else {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                 "%s: invalid parity",
                                 error_func);
                    }
                } else {
                    if (error_msg) {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                 "invalid parity (%s)",
                                 error_msg);
                    } else {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                 "invalid parity");
                    }
                }

                break;
            case UART_ESTOP:
                if (error_func) {
                    if (error_msg) {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                 "%s: invalid stop bits (%s)",
                                 error_func,
                                 error_msg);
                    } else {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                 "%s: invalid stop bits",
                                 error_func);
                    }
                } else {
                    if (error_msg) {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                 "invalid stop bits (%s)",
                                 error_msg);
                    } else {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                 "invalid stop bits");
                    }
                }

                break;
            case UART_EFLOW:
                if (error_func) {
                    if (error_msg) {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                 "%s: invalid flow control (%s)",
                                 error_func,
                                 error_msg);
                    } else {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                 "%s: invalid flow control",
                                 error_func);
                    }
                } else {
                    if (error_msg) {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                 "invalid flow control (%s)",
                                 error_msg);
                    } else {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                 "invalid flow control");
                    }
                }

                break;
            case UART_EPIN:
                if (error_func) {
                    if (error_msg) {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                 "%s: invalid pin (%s)",
                                 error_func,
                                 error_msg);
                    } else {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                 "%s: invalid pin",
                                 error_func);
                    }
                } else {
                    if (error_msg) {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                 "invalid pin (%s)",
                                 error_msg);
                    } else {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                 "invalid pin");
                    }
                }

                break;
            case UART_EPERM:
                if (error_func) {
                    if (error_msg) {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                 "%s: insufficient permissions (%s)",
                                 error_func,
                                 error_msg);
                    } else {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                 "%s: insufficient permissions",
                                 error_func);
                    }
                } else {
                    if (error_msg) {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                 "insufficient permissions (%s)",
                                 error_msg);
                    } else {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                 "insufficient permissions");
                    }
                }

                break;
            case UART_EHANDLE:
                if (error_func) {
                    if (error_msg) {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                 "%s: invalid handle (%s)",
                                 error_func,
                                 error_msg);
                    } else {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                 "%s: invalid handle",
                                 error_func);
                    }
                } else {
                    if (error_msg) {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                 "invalid handle (%s)",
                                 error_msg);
                    } else {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                 "invalid handle");
                    }
                }

                break;
            case UART_ECTX:
                if (error_func) {
                    if (error_msg) {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                 "%s: invalid context (%s)",
                                 error_func,
                                 error_msg);
                    } else {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                 "%s: invalid context",
                                 error_func);
                    }
                } else {
                    if (error_msg) {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                 "invalid context (%s)",
                                 error_msg);
                    } else {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                 "invalid context");
                    }
                }

                break;
            default:
                if (error_func) {
                    if (error_msg) {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                 "%s: unknown error code (%d) (%s)",
                                 error_func,
                                 uart->error,
                                 error_msg);
                    } else {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                 "%s: unknown error code (%d)",
                                 error_func,
                                 uart->error);
                    }
                } else {
                    if (error_msg) {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                 "unknown error code (%d) (%s)",
                                 uart->error,
                                 error_msg);
                    } else {
                        snprintf(uart->errormsg, UART_ERRORMAX,
                                 "unknown error code (%d)",
                                 uart->error);
                    }
                }
        }
    }
}

void _uart_perror(struct _uart_ctx *ctx,
                  struct _uart *uart)
{
    if (!ctx) {
        return;
    }

    if (!uart) {
        fprintf(stderr, "%s: context error: %s",
                LIBUART_NAME,
                ctx->errormsg);
    } else {
        fprintf(stderr, "%s: %s: device error: %s",
                LIBUART_NAME,
                uart->dev,
                uart->errormsg);
    }
}
