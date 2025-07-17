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

#include "error.h"
#include "version.h"
#include "util.h"

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

int _uart_init_baud(struct _uart *uart)
{
    int ret;
    struct termios options;
    
    ret = tcgetattr(uart->fd, &options);
    
    if (ret == -1) {
        uart->error = UART_ESYSTEM;
        _uart_error(uart, __func__, "tcgetattr() failed");
        return -1;
    }
    
    switch (uart->baud) {
    case UART_BAUD_0:
        ret = cfsetispeed(&options, B0);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetispeed() failed");
            return -1;
        }
        
        ret = cfsetospeed(&options, B0);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetospeed() failed");
            return -1;
        }

        break;
    case UART_BAUD_50:
        ret = cfsetispeed(&options, B50);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetispeed() failed");
            return -1;
        }
        
        ret = cfsetospeed(&options, B50);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetospeed() failed");
            return -1;
        }

        break;
    case UART_BAUD_75:
        ret = cfsetispeed(&options, B75);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetispeed() failed");
            return -1;
        }
        
        ret = cfsetospeed(&options, B75);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetospeed() failed");
            return -1;
        }

        break;
    case UART_BAUD_110:
        ret = cfsetispeed(&options, B110);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetispeed() failed");
            return -1;
        }
        
        ret = cfsetospeed(&options, B110);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetospeed() failed");
            return -1;
        }
        
        break;
    case UART_BAUD_134:
        ret = cfsetispeed(&options, B134);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetispeed() failed");
            return -1;
        }
        
        ret = cfsetospeed(&options, B134);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetospeed() failed");
            return -1;
        }

        break;
    case UART_BAUD_150:
        ret = cfsetispeed(&options, B150);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetispeed() failed");
            return -1;
        }
        
        ret = cfsetospeed(&options, B150);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetospeed() failed");
            return -1;
        }

        break;
    case UART_BAUD_200:
        ret = cfsetispeed(&options, B200);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetispeed() failed");
            return -1;
        }
        
        ret = cfsetospeed(&options, B200);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetospeed() failed");
            return -1;
        }

        break;
    case UART_BAUD_300:
        ret = cfsetispeed(&options, B300);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetispeed() failed");
            return -1;
        }
        
        ret = cfsetospeed(&options, B300);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetospeed() failed");
            return -1;
        }
        
        break;
    case UART_BAUD_600:
        ret = cfsetispeed(&options, B600);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetispeed() failed");
            return -1;
        }
        
        ret = cfsetospeed(&options, B600);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetospeed() failed");
            return -1;
        }
        
        break;
    case UART_BAUD_1200:
        ret = cfsetispeed(&options, B1200);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetispeed() failed");
            return -1;
        }
        
        ret = cfsetospeed(&options, B1200);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetospeed() failed");
            return -1;
        }
        
        break;
    case UART_BAUD_1800:
        ret = cfsetispeed(&options, B1800);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetispeed() failed");
            return -1;
        }
        
        ret = cfsetospeed(&options, B1800);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetospeed() failed");
            return -1;
        }

        break;
    case UART_BAUD_2400:
        ret = cfsetispeed(&options, B2400);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetispeed() failed");
            return -1;
        }
        
        ret = cfsetospeed(&options, B2400);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetospeed() failed");
            return -1;
        }
        
        break;
    case UART_BAUD_4800:
        ret = cfsetispeed(&options, B4800);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetispeed() failed");
            return -1;
        }
        
        ret = cfsetospeed(&options, B4800);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetospeed() failed");
            return -1;
        }
        
        break;
    case UART_BAUD_9600:
        ret = cfsetispeed(&options, B9600);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetispeed() failed");
            return -1;
        }
        
        ret = cfsetospeed(&options, B9600);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetospeed() failed");
            return -1;
        }
        
        break;
    case UART_BAUD_19200:
        ret = cfsetispeed(&options, B19200);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetispeed() failed");
            return -1;
        }
        
        ret = cfsetospeed(&options, B19200);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetospeed() failed");
            return -1;
        }
        
        break;
    case UART_BAUD_38400:
        ret = cfsetispeed(&options, B38400);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetispeed() failed");
            return -1;
        }
        
        ret = cfsetospeed(&options, B38400);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetospeed() failed");
            return -1;
        }
        
        break;
    case UART_BAUD_57600:
        ret = cfsetispeed(&options, B57600);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetispeed() failed");
            return -1;
        }
        
        ret = cfsetospeed(&options, B57600);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetospeed() failed");
            return -1;
        }
        
        break;
    case UART_BAUD_115200:
        ret = cfsetispeed(&options, B115200);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetispeed() failed");
            return -1;
        }
        
        ret = cfsetospeed(&options, B115200);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetospeed() failed");
            return -1;
        }
        
        break;
    case UART_BAUD_230400:
        ret = cfsetispeed(&options, B230400);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetispeed() failed");
            return -1;
        }
        
        ret = cfsetospeed(&options, B230400);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetospeed() failed");
            return -1;
        }

        break;
    case UART_BAUD_460800:
        ret = cfsetispeed(&options, B460800);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetispeed() failed");
            return -1;
        }
        
        ret = cfsetospeed(&options, B460800);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetospeed() failed");
            return -1;
        }

        break;
    case UART_BAUD_500000:
        ret = cfsetispeed(&options, B500000);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetispeed() failed");
            return -1;
        }
        
        ret = cfsetospeed(&options, B500000);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetospeed() failed");
            return -1;
        }

        break;
    case UART_BAUD_576000:
        ret = cfsetispeed(&options, B576000);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetispeed() failed");
            return -1;
        }
        
        ret = cfsetospeed(&options, B576000);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetospeed() failed");
            return -1;
        }

        break;
    case UART_BAUD_921600:
        ret = cfsetispeed(&options, B921600);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetispeed() failed");
            return -1;
        }
        
        ret = cfsetospeed(&options, B921600);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetospeed() failed");
            return -1;
        }

        break;
    case UART_BAUD_1000000:
        ret = cfsetispeed(&options, B1000000);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetispeed() failed");
            return -1;
        }
        
        ret = cfsetospeed(&options, B1000000);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetospeed() failed");
            return -1;
        }

        break;
    case UART_BAUD_1152000:
        ret = cfsetispeed(&options, B1152000);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetispeed() failed");
            return -1;
        }
        
        ret = cfsetospeed(&options, B1152000);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetospeed() failed");
            return -1;
        }

        break;
    case UART_BAUD_1500000:
        ret = cfsetispeed(&options, B1500000);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetispeed() failed");
            return -1;
        }
        
        ret = cfsetospeed(&options, B1500000);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetospeed() failed");
            return -1;
        }

        break;
    case UART_BAUD_2000000:
        ret = cfsetispeed(&options, B2000000);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetispeed() failed");
            return -1;
        }
        
        ret = cfsetospeed(&options, B2000000);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetospeed() failed");
            return -1;
        }

        break;
    case UART_BAUD_2500000:
        ret = cfsetispeed(&options, B2500000);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetispeed() failed");
            return -1;
        }
        
        ret = cfsetospeed(&options, B2500000);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetospeed() failed");
            return -1;
        }

        break;
    case UART_BAUD_3000000:
        ret = cfsetispeed(&options, B3000000);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetispeed() failed");
            return -1;
        }
        
        ret = cfsetospeed(&options, B3000000);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetospeed() failed");
            return -1;
        }

        break;
    case UART_BAUD_3500000:
        ret = cfsetispeed(&options, B3500000);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetispeed() failed");
            return -1;
        }
        
        ret = cfsetospeed(&options, B3500000);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetospeed() failed");
            return -1;
        }

        break;
    case UART_BAUD_4000000:
        ret = cfsetispeed(&options, B4000000);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetispeed() failed");
            return -1;
        }
        
        ret = cfsetospeed(&options, B4000000);
        
        if (ret == -1) {
            uart->error = UART_ESYSTEM;
            _uart_error(uart, __func__, "cfsetospeed() failed");
            return -1;
        }

        break;
    default:
        uart->error = UART_EBAUD;
        _uart_error(uart, __func__, "invalid/unsupported baud rate");
        return -1;
    }
    
    ret = tcsetattr(uart->fd, TCSANOW, &options);
    
    if (ret == -1) {
        uart->error = UART_ESYSTEM;
        _uart_error(uart, __func__, "tcsetattr() failed");
        return -1;
    }
    
    uart->error = UART_ESUCCESS;
    return 0;
}

int _uart_init_databits(struct _uart *uart)
{
    int ret;
    struct termios options;
    
    ret = tcgetattr(uart->fd, &options);
    
    if (ret == -1) {
        uart->error = UART_ESYSTEM;
        _uart_error(uart, __func__, "tcgetattr() failed");
        return -1;
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
        uart->error = UART_EDATA;
        _uart_error(uart, __func__, "invalid/unsupported data bits");
        return -1;
    }
    
    ret = tcsetattr(uart->fd, TCSANOW, &options);
    
    if (ret == -1) {
        uart->error = UART_ESYSTEM;
        _uart_error(uart, __func__, "tcsetattr() failed");
        return -1;
    }
    
    uart->error = UART_ESUCCESS;
    return 0;
}

int _uart_init_parity(struct _uart *uart)
{
    int ret;
    struct termios options;
    
    ret = tcgetattr(uart->fd, &options);
    
    if (ret == -1) {
        uart->error = UART_ESYSTEM;
        _uart_error(uart, __func__, "tcgetattr() failed");
        return -1;
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
        uart->error = UART_EPARITY;
        _uart_error(uart, __func__, "invalid/unsupported parity");
        return -1;
    }
    
    ret = tcsetattr(uart->fd, TCSANOW, &options);
    
    if (ret == -1) {
        uart->error = UART_ESYSTEM;
        _uart_error(uart, __func__, "tcsetattr() failed");
        return -1;
    }
    
    uart->error = UART_ESUCCESS;
    return 0;
}

int _uart_init_stopbits(struct _uart *uart)
{
    int ret;
    struct termios options;
    
    ret = tcgetattr(uart->fd, &options);
    
    if (ret == -1) {
        uart->error = UART_ESYSTEM;
        _uart_error(uart, __func__, "tcgetattr() failed");
        return -1;
    }
    
    switch (uart->stop_bits) {
    case UART_STOP_1_0:
        options.c_cflag &= ~CSTOPB;
        break;
    case UART_STOP_2_0:
        options.c_cflag |= CSTOPB;
        break;
    default:
        uart->error = UART_ESTOP;
        _uart_error(uart, __func__, "invalid/unsupported stop bits");
        return -1;
    }
    
    ret = tcsetattr(uart->fd, TCSANOW, &options);
    
    if (ret == -1) {
        uart->error = UART_ESYSTEM;
        _uart_error(uart, __func__, "tcsetattr() failed");
        return -1;
    }
    
    uart->error = UART_ESUCCESS;
    return 0;
}

int _uart_init_flow(struct _uart *uart)
{
    int ret;
    struct termios options;
    
    ret = tcgetattr(uart->fd, &options);
    
    if (ret == -1) {
        uart->error = UART_ESYSTEM;
        _uart_error(uart, __func__, "tcgetattr() failed");
        return -1;
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
        uart->error = UART_EFLOW;
        _uart_error(uart, __func__, "invalid/unsupported flow control");
        return -1;
    }
    
    ret = tcsetattr(uart->fd, TCSANOW, &options);
    
    if (ret == -1) {
        uart->error = UART_ESYSTEM;
        _uart_error(uart, __func__, "tcsetattr() failed");
        return -1;
    }
    
    uart->error = UART_ESUCCESS;
    return 0;
}

int _uart_init(void)
{
    int ret;
    int i;
    gid_t g_list[256];
    struct group *g;

    ret = getgroups(256, g_list);

    /* User should be in group dialout */
    g = getgrnam("dialout");

    if (!g) {
        return UART_ESYSTEM;
    }

    for (i = 0; i < ret; i++) {
        if (g_list[i] == g->gr_gid) {
            return UART_ESUCCESS;;
        }
    }

    return UART_EPERM;
}

ssize_t _uart_get_device_list(size_t len, struct _uart **ret_uarts)
{
    FILE *f_drivers;

    f_drivers = fopen("proc/tty/drivers", "rb");

    if (!f_drivers) {
        return 0;
    }

    return 0;
}

int _uart_open(struct _uart *uart)
{
    int ret;
    int fd;
    struct termios options;
    
    fd = open(uart->dev, O_RDWR | O_NOCTTY | O_NDELAY);
    
    if (fd == -1) {
        uart->error = UART_ESYSTEM;
        _uart_error(uart, __func__, "open() failed");
        return -1;
    }
    
    uart->fd = fd;

    /* set non-blocking mode */
    ret = fcntl(uart->fd, F_SETFL, O_NDELAY);

    if (ret == -1) {
        uart->error = UART_ESYSTEM;
        _uart_error(uart, __func__, "fcntl() failed");
        return -1;
    }

    /* set baud rate */
    ret = _uart_init_baud(uart);

    if (ret == -1)
        return -1;

    /* set data bits */
    ret = _uart_init_databits(uart);

    if (ret == -1)
        return -1;

    /* set parity */
    ret = _uart_init_parity(uart);

    if (ret == -1)
        return -1;

    /* set stop bits */
    ret = _uart_init_stopbits(uart);

    if (ret == -1)
        return -1;

    /* set flow control */
    ret = _uart_init_flow(uart);

    if (ret == -1)
        return -1;

    ret = tcgetattr(uart->fd, &options);

    if (ret == -1) {
        uart->error = UART_ESYSTEM;
        _uart_error(uart, __func__, "tcgetattr() failed");
        return -1;
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
        uart->error = UART_ESYSTEM;
        _uart_error(uart, __func__, "tcsetattr() failed");
        return -1;
    }
    
    uart->error = UART_ESUCCESS;
    return 0;
}

void _uart_close(struct _uart *uart)
{
    close(uart->fd);
}

ssize_t _uart_send(struct _uart *uart, void *send_buf, size_t len)
{
    ssize_t ret;
    
    ret = write(uart->fd, send_buf, len);
    
    if (ret == -1) {
        uart->error = UART_ESYSTEM;
        _uart_error(uart, __func__, "write() failed");
        return -1;
    }
    
    if (ret != (ssize_t) len) {
        uart->error = UART_ESYSTEM;
        _uart_error(uart, __func__, "write() could not write all data");
        return ret;
    }
    
    uart->error = UART_ESUCCESS;
    return ret;
}

ssize_t _uart_recv(struct _uart *uart, void *recv_buf, size_t len)
{
    ssize_t ret = 0;
    
    ret = read(uart->fd, recv_buf, len);
    
    if (ret == -1) {
        uart->error = UART_ESYSTEM;
        _uart_error(uart, __func__, "read() failed");
        return -1;
    }
    
    uart->error = UART_ESUCCESS;

    return ret;
}

int _uart_flush(struct _uart *uart)
{
    int ret = 0;
    
    ret = fsync(uart->fd);
    
    if (ret == -1) {
        uart->error = UART_ESYSTEM;
        _uart_error(uart, __func__, "fsync() failed");
        return -1;
    }
    
    uart->error = UART_ESUCCESS;
    return 0;
}

int _uart_set_pin(struct _uart *uart, enum e_pins pin, int state)
{
    int ret;
    int status;
    
    ret = ioctl(uart->fd, TIOCMGET, &status);
    
    if (ret == -1) {
        uart->error = UART_ESYSTEM;
        _uart_error(uart, __func__, "ioctl() failed");
        return -1;
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
        uart->error = UART_EPIN;
        _uart_error(uart, __func__, "invalid pin");
        return -1;
    }
    
    ioctl(uart->fd, TIOCMSET, status);
    
    if (ret == -1) {
        uart->error = UART_ESYSTEM;
        _uart_error(uart, __func__, "ioctl() failed");
        return -1;
    }
    
    uart->error = UART_ESUCCESS;
    return 0;
}

int _uart_get_pin(struct _uart *uart, enum e_pins pin, int *state)
{
    int ret = 0;
    int status;
    
    ioctl(uart->fd, TIOCMGET, &status);
    
    if (ret == -1) {
        uart->error = UART_ESYSTEM;
        _uart_error(uart, __func__, "ioctl() failed");
        return -1;
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
        uart->error = UART_EPIN;
        _uart_error(uart, __func__, "invalid pin");
        return -1;
    }
    
    (*state) = ret;
    uart->error = UART_ESUCCESS;
    return 0;
}

int _uart_get_bytes(struct _uart *uart, int *bytes)
{
    int ret = 0;
    
    ioctl(uart->fd, FIONREAD, &ret);
    
    if (ret == -1) {
        uart->error = UART_ESYSTEM;
        _uart_error(uart, __func__, "ioctl() failed");
        return -1;
    }
    
    (*bytes) = ret;
    uart->error = UART_ESUCCESS;
    return 0;
}
