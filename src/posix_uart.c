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
#include <fcntl.h>
#include <unistd.h>
#include <grp.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <dirent.h>

#include "_uart.h"
#include "_util.h"

#include <UART.h>

int _uart_baud_valid(int value)
{
    int E[] = {
        UART_BAUD_0,
        UART_BAUD_50,
        UART_BAUD_75,
        UART_BAUD_110,
        UART_BAUD_134,
        UART_BAUD_150,
        UART_BAUD_200,
        UART_BAUD_300,
        UART_BAUD_600,
        UART_BAUD_1200,
        UART_BAUD_1800,
        UART_BAUD_2400,
        UART_BAUD_4800,
        UART_BAUD_9600,
        UART_BAUD_19200,
        UART_BAUD_38400,
        UART_BAUD_57600,
        UART_BAUD_115200,
        UART_BAUD_230400,
        UART_BAUD_460800,
        UART_BAUD_500000,
        UART_BAUD_576000,
        UART_BAUD_921600,
        UART_BAUD_1000000,
        UART_BAUD_1152000,
        UART_BAUD_1500000,
        UART_BAUD_2000000,
        UART_BAUD_2500000,
        UART_BAUD_3000000,
        UART_BAUD_3500000,
        UART_BAUD_4000000
    };
    
    if (enum_contains(E, sizeof(E)/sizeof(E[0]), value))
        return 1;
    
    return 0;
}

int _uart_init_baud(struct _uart_ctx *ctx, struct _uart *uart)
{
    int ret;
    struct termios options;
    
    if (!ctx) {
        return UART_ECTX;
    }

    if (!uart) {
        _uart_error(ctx, NULL, UART_EHANDLE, NULL, "NULL");

        return UART_EHANDLE;
    }

    ret = tcgetattr(uart->fd, &options);
    
    if (ret == -1) {
        _uart_error(ctx, uart, UART_ESYSAPI, "tcgetattr", NULL);

        return UART_ESYSAPI;
    }
    
    switch (uart->baud) {
    case UART_BAUD_0:
        ret = cfsetispeed(&options, B0);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetispeed", NULL);

            return UART_ESYSAPI;
        }
        
        ret = cfsetospeed(&options, B0);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetospeed", NULL);

            return UART_ESYSAPI;
        }

        break;
    case UART_BAUD_50:
        ret = cfsetispeed(&options, B50);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetispeed", NULL);

            return UART_ESYSAPI;
        }
        
        ret = cfsetospeed(&options, B50);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetospeed", NULL);

            return UART_ESYSAPI;
        }

        break;
    case UART_BAUD_75:
        ret = cfsetispeed(&options, B75);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetispeed", NULL);

            return UART_ESYSAPI;
        }
        
        ret = cfsetospeed(&options, B75);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetospeed", NULL);

            return UART_ESYSAPI;
        }

        break;
    case UART_BAUD_110:
        ret = cfsetispeed(&options, B110);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetispeed", NULL);

            return UART_ESYSAPI;
        }
        
        ret = cfsetospeed(&options, B110);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetospeed", NULL);

            return UART_ESYSAPI;
        }
        
        break;
    case UART_BAUD_134:
        ret = cfsetispeed(&options, B134);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetispeed", NULL);

            return UART_ESYSAPI;
        }
        
        ret = cfsetospeed(&options, B134);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetospeed", NULL);

            return UART_ESYSAPI;
        }

        break;
    case UART_BAUD_150:
        ret = cfsetispeed(&options, B150);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetispeed", NULL);

            return UART_ESYSAPI;
        }
        
        ret = cfsetospeed(&options, B150);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetospeed", NULL);

            return UART_ESYSAPI;
        }

        break;
    case UART_BAUD_200:
        ret = cfsetispeed(&options, B200);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetispeed", NULL);

            return UART_ESYSAPI;
        }
        
        ret = cfsetospeed(&options, B200);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetospeed", NULL);

            return UART_ESYSAPI;
        }

        break;
    case UART_BAUD_300:
        ret = cfsetispeed(&options, B300);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetispeed", NULL);

            return UART_ESYSAPI;
        }
        
        ret = cfsetospeed(&options, B300);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetospeed", NULL);

            return UART_ESYSAPI;
        }
        
        break;
    case UART_BAUD_600:
        ret = cfsetispeed(&options, B600);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetispeed", NULL);

            return UART_ESYSAPI;
        }
        
        ret = cfsetospeed(&options, B600);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetospeed", NULL);

            return UART_ESYSAPI;
        }
        
        break;
    case UART_BAUD_1200:
        ret = cfsetispeed(&options, B1200);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetispeed", NULL);

            return UART_ESYSAPI;
        }
        
        ret = cfsetospeed(&options, B1200);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetospeed", NULL);

            return UART_ESYSAPI;
        }
        
        break;
    case UART_BAUD_1800:
        ret = cfsetispeed(&options, B1800);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetispeed", NULL);

            return UART_ESYSAPI;
        }
        
        ret = cfsetospeed(&options, B1800);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetospeed", NULL);

            return UART_ESYSAPI;
        }

        break;
    case UART_BAUD_2400:
        ret = cfsetispeed(&options, B2400);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetispeed", NULL);

            return UART_ESYSAPI;
        }
        
        ret = cfsetospeed(&options, B2400);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetospeed", NULL);

            return UART_ESYSAPI;
        }
        
        break;
    case UART_BAUD_4800:
        ret = cfsetispeed(&options, B4800);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetispeed", NULL);

            return UART_ESYSAPI;
        }
        
        ret = cfsetospeed(&options, B4800);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetospeed", NULL);

            return UART_ESYSAPI;
        }
        
        break;
    case UART_BAUD_9600:
        ret = cfsetispeed(&options, B9600);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetispeed", NULL);

            return UART_ESYSAPI;
        }
        
        ret = cfsetospeed(&options, B9600);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetospeed", NULL);

            return UART_ESYSAPI;
        }
        
        break;
    case UART_BAUD_19200:
        ret = cfsetispeed(&options, B19200);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetispeed", NULL);

            return UART_ESYSAPI;
        }
        
        ret = cfsetospeed(&options, B19200);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetospeed", NULL);

            return UART_ESYSAPI;
        }
        
        break;
    case UART_BAUD_38400:
        ret = cfsetispeed(&options, B38400);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetispeed", NULL);

            return UART_ESYSAPI;
        }
        
        ret = cfsetospeed(&options, B38400);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetospeed", NULL);

            return UART_ESYSAPI;
        }
        
        break;
    case UART_BAUD_57600:
        ret = cfsetispeed(&options, B57600);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetispeed", NULL);

            return UART_ESYSAPI;
        }
        
        ret = cfsetospeed(&options, B57600);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetospeed", NULL);

            return UART_ESYSAPI;
        }
        
        break;
    case UART_BAUD_115200:
        ret = cfsetispeed(&options, B115200);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetispeed", NULL);

            return UART_ESYSAPI;
        }
        
        ret = cfsetospeed(&options, B115200);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetospeed", NULL);

            return UART_ESYSAPI;
        }
        
        break;
    case UART_BAUD_230400:
        ret = cfsetispeed(&options, B230400);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetispeed", NULL);

            return UART_ESYSAPI;
        }
        
        ret = cfsetospeed(&options, B230400);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetospeed", NULL);

            return UART_ESYSAPI;
        }

        break;
    case UART_BAUD_460800:
        ret = cfsetispeed(&options, B460800);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetispeed", NULL);

            return UART_ESYSAPI;
        }
        
        ret = cfsetospeed(&options, B460800);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetospeed", NULL);

            return UART_ESYSAPI;
        }

        break;
    case UART_BAUD_500000:
        ret = cfsetispeed(&options, B500000);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetispeed", NULL);

            return UART_ESYSAPI;
        }
        
        ret = cfsetospeed(&options, B500000);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetospeed", NULL);

            return UART_ESYSAPI;
        }

        break;
    case UART_BAUD_576000:
        ret = cfsetispeed(&options, B576000);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetispeed", NULL);

            return UART_ESYSAPI;
        }
        
        ret = cfsetospeed(&options, B576000);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetospeed", NULL);

            return UART_ESYSAPI;
        }

        break;
    case UART_BAUD_921600:
        ret = cfsetispeed(&options, B921600);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetispeed", NULL);

            return UART_ESYSAPI;;
        }
        
        ret = cfsetospeed(&options, B921600);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetospeed", NULL);

            return UART_ESYSAPI;
        }

        break;
    case UART_BAUD_1000000:
        ret = cfsetispeed(&options, B1000000);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetispeed", NULL);

            return UART_ESYSAPI;
        }
        
        ret = cfsetospeed(&options, B1000000);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetospeed", NULL);

            return UART_ESYSAPI;
        }

        break;
    case UART_BAUD_1152000:
        ret = cfsetispeed(&options, B1152000);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetispeed", NULL);

            return UART_ESYSAPI;
        }
        
        ret = cfsetospeed(&options, B1152000);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetospeed", NULL);

            return UART_ESYSAPI;
        }

        break;
    case UART_BAUD_1500000:
        ret = cfsetispeed(&options, B1500000);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetispeed", NULL);

            return UART_ESYSAPI;
        }
        
        ret = cfsetospeed(&options, B1500000);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetospeed", NULL);

            return UART_ESYSAPI;
        }

        break;
    case UART_BAUD_2000000:
        ret = cfsetispeed(&options, B2000000);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetispeed", NULL);

            return UART_ESYSAPI;
        }
        
        ret = cfsetospeed(&options, B2000000);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetospeed", NULL);

            return UART_ESYSAPI;
        }

        break;
    case UART_BAUD_2500000:
        ret = cfsetispeed(&options, B2500000);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetispeed", NULL);

            return UART_ESYSAPI;
        }
        
        ret = cfsetospeed(&options, B2500000);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetospeed", NULL);

            return UART_ESYSAPI;
        }

        break;
    case UART_BAUD_3000000:
        ret = cfsetispeed(&options, B3000000);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetispeed", NULL);

            return UART_ESYSAPI;
        }
        
        ret = cfsetospeed(&options, B3000000);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetospeed", NULL);

            return UART_ESYSAPI;
        }

        break;
    case UART_BAUD_3500000:
        ret = cfsetispeed(&options, B3500000);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetispeed", NULL);

            return UART_ESYSAPI;
        }
        
        ret = cfsetospeed(&options, B3500000);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetospeed", NULL);

            return UART_ESYSAPI;
        }

        break;
    case UART_BAUD_4000000:
        ret = cfsetispeed(&options, B4000000);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetispeed", NULL);

            return UART_ESYSAPI;
        }
        
        ret = cfsetospeed(&options, B4000000);
        
        if (ret == -1) {
            _uart_error(ctx, uart, UART_ESYSAPI, "cfsetospeed", NULL);

            return UART_ESYSAPI;
        }

        break;
    default:
        _uart_error(ctx, uart, UART_EBAUD, NULL, NULL);

        return UART_EBAUD;
    }
    
    ret = tcsetattr(uart->fd, TCSANOW, &options);
    
    if (ret == -1) {
        _uart_error(ctx, uart, UART_ESYSAPI, "tcsetattr", NULL);

        return UART_ESYSAPI;
    }
    
    return UART_ESUCCESS;
}

int _uart_init_databits(struct _uart_ctx *ctx, struct _uart *uart)
{
    int ret;
    struct termios options;
    
    if (!ctx) {
        return UART_ECTX;
    }

    if (!uart) {
        _uart_error(ctx, NULL, UART_EHANDLE, NULL, "NULL");

        return UART_EHANDLE;
    }

    ret = tcgetattr(uart->fd, &options);
    
    if (ret == -1) {
        _uart_error(ctx, uart, UART_ESYSAPI, "tcgetattr", NULL);

        return UART_ESYSAPI;
    }
    
    switch (uart->data_bits) {
    case UART_DATA_5:
        options.c_cflag &= ~CSIZE;
        options.c_cflag |= CS5;
        break;
    case UART_DATA_6:
        options.c_cflag &= ~CSIZE;
        options.c_cflag |= CS6;
        break;
    case UART_DATA_7:
        options.c_cflag &= ~CSIZE;
        options.c_cflag |= CS7;
        break;
    case UART_DATA_8:
        options.c_cflag &= ~CSIZE;
        options.c_cflag |= CS8;
        break;
    default:
        _uart_error(ctx, uart, UART_EDATA, NULL, "unsupported");

        return UART_EDATA;
    }
    
    ret = tcsetattr(uart->fd, TCSANOW, &options);
    
    if (ret == -1) {
        _uart_error(ctx, uart, UART_ESYSAPI, "tcsetattr", NULL);

        return UART_ESYSAPI;
    }
    
    return UART_ESUCCESS;
}

int _uart_init_parity(struct _uart_ctx *ctx, struct _uart *uart)
{
    int ret;
    struct termios options;
    
    if (!ctx) {
        return UART_ECTX;
    }

    if (!uart) {
        _uart_error(ctx, NULL, UART_EHANDLE, NULL, "NULL");

        return UART_EHANDLE;
    }

    ret = tcgetattr(uart->fd, &options);
    
    if (ret == -1) {
        _uart_error(ctx, uart, UART_ESYSAPI, "tcgetattr", NULL);

        return UART_ESYSAPI;
    }
    
    switch (uart->parity) {
    case UART_PARITY_NONE:
        options.c_cflag &= ~PARENB;
        break;
    case UART_PARITY_ODD:
        options.c_cflag |= PARENB;
        options.c_cflag |= PARODD;
        break;
    case UART_PARITY_EVEN:
        options.c_cflag |= PARENB;
        options.c_cflag &= ~PARODD;
        break;
    default:
        _uart_error(ctx, uart, UART_EPARITY, NULL, "unsupported");

        return UART_EPARITY;
    }
    
    ret = tcsetattr(uart->fd, TCSANOW, &options);
    
    if (ret == -1) {
        _uart_error(ctx, uart, UART_ESYSAPI, "tcsetattr", NULL);

        return UART_ESYSAPI;
    }
    
    return UART_ESUCCESS;
}

int _uart_init_stopbits(struct _uart_ctx *ctx, struct _uart *uart)
{
    int ret;
    struct termios options;
    
    if (!ctx) {
        return UART_ECTX;
    }

    if (!uart) {
        _uart_error(ctx, NULL, UART_EHANDLE, NULL, "NULL");

        return UART_EHANDLE;
    }

    ret = tcgetattr(uart->fd, &options);
    
    if (ret == -1) {
        _uart_error(ctx, uart, UART_ESYSAPI, "tcgetattr", NULL);

        return UART_ESYSAPI;
    }
    
    switch (uart->stop_bits) {
    case UART_STOP_1_0:
        options.c_cflag &= ~CSTOPB;
        break;
    case UART_STOP_2_0:
        options.c_cflag |= CSTOPB;
        break;
    default:
        _uart_error(ctx, uart, UART_ESTOP, NULL, "unsupported");

        return UART_ESTOP;
    }
    
    ret = tcsetattr(uart->fd, TCSANOW, &options);
    
    if (ret == -1) {
        _uart_error(ctx, uart, UART_ESYSAPI, "tcsetattr", NULL);

        return UART_ESYSAPI;
    }
    
    return UART_ESUCCESS;
}

int _uart_init_flow(struct _uart_ctx *ctx, struct _uart *uart)
{
    int ret;
    struct termios options;
    
    if (!ctx) {
        return UART_ECTX;
    }

    if (!uart) {
        _uart_error(ctx, NULL, UART_EHANDLE, NULL, "NULL");

        return UART_EHANDLE;
    }

    ret = tcgetattr(uart->fd, &options);
    
    if (ret == -1) {
        _uart_error(ctx, uart, UART_ESYSAPI, "tcgetattr", NULL);

        return UART_ESYSAPI;
    }
    
    switch (uart->flow_ctrl) {
    case UART_FLOW_NO:
        options.c_iflag &= ~(IXON | IXOFF | IXANY);
        break;
    case UART_FLOW_SW:
        options.c_iflag |= (IXON | IXOFF | IXANY);
        break;
    case UART_FLOW_HW:
        options.c_iflag &= ~(IXON | IXOFF | IXANY);
        break;
    default:
        _uart_error(ctx, uart, UART_EFLOW, NULL, "unsupported");

        return UART_EFLOW;
    }
    
    ret = tcsetattr(uart->fd, TCSANOW, &options);
    
    if (ret == -1) {
        _uart_error(ctx, uart, UART_ESYSAPI, "tcsetattr", NULL);

        return UART_ESYSAPI;
    }
    
    return UART_ESUCCESS;
}

int _uart_init(struct _uart_ctx *ctx)
{
    int ret;
    int i;
    gid_t g_list[256];
    struct group *g;

    if (!ctx) {
        return UART_ECTX;
    }

    ret = getgroups(256, g_list);

    if (ret == -1) {
        _uart_error(ctx, NULL, UART_ESYSAPI, "getgroups", NULL);

        return UART_ESYSAPI;
    }

    /**
     * User must be in group 'dialout' for using the UART
     *
     * NOTE: On FreeBSD too?
     */
    g = getgrnam("dialout");

    if (!g) {
        _uart_error(ctx, NULL, UART_ESYSAPI, "getgrnam", NULL);

        return UART_ESYSAPI;
    }

    for (i = 0; i < ret; i++) {
        if (g_list[i] == g->gr_gid) {
            ctx->flags |= UART_CTXFOKAY;
            return UART_ESUCCESS;;
        }
    }

    ctx->init_done = 1;
    return UART_EPERM;
}

int _uart_get_device_list(struct _uart_ctx *ctx)
{
    FILE *f;
    char line[512];
    char devname[128];
    char devtype[128];
    char *line_ptr;
    char *ptr;
    char *cell_ptr;
    int count;
    DIR *dir;
    struct dirent *entry;
    char *dev[64];
    int dev_index = 0;
    int i;

    if (!ctx) {
        return UART_ECTX;
    }

    line_ptr = (char *) line;

    f = fopen("/proc/tty/drivers", "rb");

    if (!f) {
        _uart_error(ctx, NULL, UART_ESYSAPI, "fopen", NULL);

        return UART_ESYSAPI;
    }

    /**
     * Parse file '/proc/tty/drivers' for available UART's
     *
     * NOTE: Exist this file on FreeBSD too?
     */
    while (!feof(f)) {
        fread(line_ptr, 1, 1, f);

        if (*line_ptr == '\n') {
            *line_ptr = '\0';
            ptr = (char *) line;
            cell_ptr = (char *) line;

            while (*ptr != ' ') {
                ptr++;
            }

            while (*ptr == ' ') {
                ptr++;
            }

            cell_ptr = ptr;
            count = 0;

            while (*ptr != ' ') {
                count++;
                ptr++;
            }

            memset(devname, 0, 128);
            strncpy(devname, cell_ptr, count);

            while (*ptr == ' ') {
                ptr++;
            }

            while (*ptr != ' ') {
                ptr++;
            }

            while (*ptr == ' ') {
                ptr++;
            }

            while (*ptr != ' ') {
                ptr++;
            }

            while (*ptr == ' ') {
                ptr++;
            }

            cell_ptr = ptr;
            count = 0;

            while (*ptr != '\0') {
                count++;
                ptr++;
            }

            memset(devtype, 0, 128);
            strncpy(devtype, cell_ptr, count);

            /**
             * Test if device is an UART
             */
            if (strcmp(devtype, "serial") == 0) {
                dev[dev_index] = malloc(strlen(strrchr(devname, '/') + 1) + 1);
                strcpy(dev[dev_index], strrchr(devname, '/') + 1);
                dev_index++;
            }

            line_ptr = (char *) line;
        } else {
            line_ptr++;
        }


    }

    fclose(f);

    if (dev_index == 0) {
        _uart_error(ctx, NULL, UART_EDEV, NULL, "no UART devices found");

        return UART_EDEV;
    }

    /**
     * Parse directory '/sys/class/tty' for available UART's
     *
     * NOTE: Exist this file on FreeBSD too?
     */
    dir = opendir("/sys/class/tty");

    if (!dir) {
        _uart_error(ctx, NULL, UART_ESYSAPI, "opendir", NULL);

        return UART_ESYSAPI;
    }

    entry = readdir(dir);

    while (entry) {
        for (i = 0; i < dev_index; i++) {
            if (strncmp(entry->d_name, dev[i], strlen(dev[i])) == 0) {
                snprintf(ctx->uarts[ctx->uarts_count]->dev, UART_NAMEMAX, "/dev/%s", entry->d_name);
                ctx->uarts_count++;
            }
        }

        entry = readdir(dir);
    }

    closedir(dir);

    for (i = 0; i < dev_index; i++) {
        free(dev[i]);
    }

    return UART_ESUCCESS;
}

int _uart_open(struct _uart_ctx *ctx, struct _uart *uart)
{
    int ret;
    int fd;
    struct termios options;
    
    if (!ctx) {
        return UART_ECTX;
    }

    if (!uart) {
        _uart_error(ctx, NULL, UART_EHANDLE, NULL, "NULL");

        return UART_EHANDLE;
    }

    fd = open(uart->dev, O_RDWR | O_NOCTTY | O_NDELAY);
    
    if (fd == -1) {
        _uart_error(ctx, uart, UART_ESYSAPI, "open", NULL);

        return UART_ESYSAPI;
    }
    
    uart->fd = fd;

    /* set non-blocking mode */
    ret = fcntl(uart->fd, F_SETFL, O_NDELAY);

    if (ret == -1) {
        _uart_error(ctx, uart, UART_ESYSAPI, "fcntl", NULL);

        return UART_ESYSAPI;
    }

    /* set baud rate */
    ret = _uart_init_baud(ctx, uart);

    if (ret != UART_ESUCCESS) {
        return ret;
    }

    /* set data bits */
    ret = _uart_init_databits(ctx, uart);

    if (ret != UART_ESUCCESS) {
        return ret;
    }

    /* set parity */
    ret = _uart_init_parity(ctx, uart);

    if (ret != UART_ESUCCESS) {
        return ret;
    }

    /* set stop bits */
    ret = _uart_init_stopbits(ctx, uart);

    if (ret != UART_ESUCCESS) {
        return ret;
    }

    /* set flow control */
    ret = _uart_init_flow(ctx, uart);

    if (ret != UART_ESUCCESS) {
        return ret;
    }

    ret = tcgetattr(uart->fd, &options);

    if (ret == -1) {
        _uart_error(ctx, uart, UART_ESYSAPI, "tcgetattr", NULL);

        return UART_ESYSAPI;
    }

    /* set raw mode (see man cfmakeraw) */
    options.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    options.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP |
    INLCR | IGNCR | ICRNL | IXON);
    options.c_oflag &= ~OPOST;
    options.c_cflag &= ~(CSIZE | PARENB);
    options.c_cflag |= CS8;

    /* enable receiver and set local mode */
    options.c_cflag |= (CLOCAL | CREAD);

    /* set raw output */
    options.c_oflag &= ~OPOST;
    ret = tcsetattr(uart->fd, TCSANOW, &options);

    if (ret == -1) {
        _uart_error(ctx, uart, UART_ESYSAPI, "tcsetattr", NULL);

        return UART_ESYSAPI;
    }
    
    return UART_ESUCCESS;
}

int _uart_close(struct _uart_ctx *ctx, struct _uart *uart)
{
    if (!ctx) {
        return UART_ECTX;
    }

    if (!uart) {
        _uart_error(ctx, NULL, UART_EHANDLE, NULL, "NULL");

        return UART_EHANDLE;
    }

    close(uart->fd);

    return UART_ESUCCESS;
}

#ifndef LIBUART_THREADS
ssize_t _uart_send(struct _uart_ctx *ctx, struct _uart *uart, void *send_buf, size_t len)
{
    ssize_t ret;
    
    if (!ctx) {
        return UART_ECTX;
    }

    if (!uart) {
        _uart_error(ctx, NULL, UART_EHANDLE, NULL, "NULL");

        return UART_EHANDLE;
    }

    ret = write(uart->fd, send_buf, len);
    
    if (ret == -1) {
        _uart_error(ctx, uart, UART_ESYSAPI, "write", NULL);

        return UART_ESYSAPI;
    }
    
    if (ret != (ssize_t) len) {
        _uart_error(ctx, uart, UART_ESYSAPI, "write", "could not send all data");

        return UART_ESYSAPI;
    }
    
    uart->error = UART_ESUCCESS;

    return ret;
}

ssize_t _uart_recv(struct _uart_ctx *ctx, struct _uart *uart, void *recv_buf, size_t len)
{
    ssize_t ret = 0;
    
    if (!ctx) {
        return UART_ECTX;
    }

    if (!uart) {
        _uart_error(ctx, NULL, UART_EHANDLE, NULL, "NULL");

        return UART_EHANDLE;
    }

    ret = read(uart->fd, recv_buf, len);
    
    if (ret == -1) {
        _uart_error(ctx, uart, UART_ESYSAPI, "read", NULL);

        return UART_ESYSAPI;
    }
    
    uart->error = UART_ESUCCESS;

    return ret;
}
#endif

int _uart_flush(struct _uart_ctx *ctx, struct _uart *uart)
{
    int ret = 0;
    
    if (!ctx) {
        return UART_ECTX;
    }

    if (!uart) {
        _uart_error(ctx, NULL, UART_EHANDLE, NULL, "NULL");

        return UART_EHANDLE;
    }

    ret = fsync(uart->fd);
    
    if (ret == -1) {
        _uart_error(ctx, uart, UART_ESYSAPI, "fsync", NULL);

        return UART_ESYSAPI;
    }
    
    return UART_ESUCCESS;
}

int _uart_set_pin(struct _uart_ctx *ctx, struct _uart *uart, enum e_pins pin, int state)
{
    int ret;
    int status;
    
    if (!ctx) {
        return UART_ECTX;
    }

    if (!uart) {
        _uart_error(ctx, NULL, UART_EHANDLE, NULL, "NULL");

        return UART_EHANDLE;
    }

    ret = ioctl(uart->fd, TIOCMGET, &status);
    
    if (ret == -1) {
        _uart_error(ctx, uart, UART_ESYSAPI, "ioctl", NULL);

        return UART_ESYSAPI;
    }
    
    switch (pin) {
    case UART_PIN_RTS:
        if (state == UART_PIN_HIGH)
            status |= TIOCM_RTS;
        else
            status &= ~TIOCM_RTS;
        break;
    case UART_PIN_DTR:
        if (state == UART_PIN_HIGH)
            status |= TIOCM_DTR;
        else
            status &= ~TIOCM_DTR;
        break;
    default:
        _uart_error(ctx, uart, UART_EPIN, NULL, NULL);

        return UART_EPIN;
    }
    
    ioctl(uart->fd, TIOCMSET, status);
    
    if (ret == -1) {
        _uart_error(ctx, uart, UART_ESYSAPI, "ioctl", NULL);

        return UART_ESYSAPI;
    }
    
    return UART_ESUCCESS;
}

int _uart_get_pin(struct _uart_ctx *ctx, struct _uart *uart, enum e_pins pin, int *state)
{
    int ret = 0;
    int status;
    
    if (!ctx) {
        return UART_ECTX;
    }

    if (!uart) {
        _uart_error(ctx, NULL, UART_EHANDLE, NULL, "NULL");

        return UART_EHANDLE;
    }

    ioctl(uart->fd, TIOCMGET, &status);
    
    if (ret == -1) {
        _uart_error(ctx, uart, UART_ESYSAPI, "ioctl", NULL);

        return UART_ESYSAPI;
    }
    
    switch (pin) {
    case UART_PIN_CTS:
        if (status & TIOCM_CTS)
            ret = UART_PIN_HIGH;
        else
            ret = UART_PIN_LOW;
        break;
    case UART_PIN_DSR:
        if (status & TIOCM_DSR)
            ret = UART_PIN_HIGH;
        else
            ret = UART_PIN_LOW;
        break;
    case UART_PIN_DCD:
        if (status & TIOCM_CAR)
            ret = UART_PIN_HIGH;
        else
            ret = UART_PIN_LOW;
        break;
    case UART_PIN_RI:
        if (status & TIOCM_RI)
            ret = UART_PIN_HIGH;
        else
            ret = UART_PIN_LOW;
        break;
    default:
        _uart_error(ctx, uart, UART_EPIN, NULL, NULL);

        return UART_EPIN;
    }
    
    (*state) = ret;

    return UART_ESUCCESS;
}

int _uart_get_bytes(struct _uart_ctx *ctx, struct _uart *uart, int *bytes)
{
    int ret = 0;
    
    if (!ctx) {
        return UART_ECTX;
    }

    if (!uart) {
        _uart_error(ctx, NULL, UART_EHANDLE, NULL, "NULL");

        return UART_EHANDLE;
    }

    ioctl(uart->fd, FIONREAD, &ret);
    
    if (ret == -1) {
        _uart_error(ctx, uart, UART_ESYSAPI, "ioctl", NULL);

        return UART_ESYSAPI;
    }
    
    (*bytes) = ret;

    return UART_ESUCCESS;
}
