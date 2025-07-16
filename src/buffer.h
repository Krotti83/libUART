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

#ifndef _LIBUART_BUFFER_H
#define _LIBUART_BUFFER_H

#include <stddef.h>

#ifdef __unix__
#include <sys/types.h>
#endif

#define BUFFER_ENONE        0
#define BUFFER_EINVAL       (-1)
#define BUFFER_ENOSPC       (-2)
#define BUFFER_EFAULT       (-3)
#define BUFFER_ERANGE       (-4)
#define BUFFER_EEMPTY       (-5)

struct _buffer;

typedef struct _buffer buffer_t;

extern buffer_t *buffer_create(ssize_t len);
extern int buffer_free(buffer_t *buf);
extern ssize_t buffer_wr(buffer_t *buf, unsigned char *data, ssize_t len);
extern ssize_t buffer_rd(buffer_t *buf, unsigned char *data, ssize_t len);
extern ssize_t buffer_get_len(buffer_t *buf);
extern ssize_t buffer_get_num(buffer_t *buf);
extern ssize_t buffer_get_free(buffer_t *buf);

#endif
