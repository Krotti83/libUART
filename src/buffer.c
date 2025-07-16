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

#include <stddef.h>
#include <stdlib.h>

#include "buffer.h"

struct _buffer {
    ssize_t b_len;
    ssize_t b_num;
    unsigned char *b_p;
    ssize_t b_idxr;
    ssize_t b_idxw;
};

buffer_t *buffer_create(ssize_t len)
{
    buffer_t *buf;
    unsigned char *p;
    
    if (len < 1) {
        return NULL;
    }
    
    p = (unsigned char *) malloc(len);
    
    if (!p) {
        return NULL;
    }
    
    buf = (buffer_t *) malloc(sizeof(buffer_t));
    
    if (!buf) {
        free(p);
        return NULL;
    }
    
    buf->b_len = len;
    buf->b_num = 0;
    buf->b_p = p;
    buf->b_idxw = 0;
    buf->b_idxr = -1;

    return buf;
}

int buffer_free(buffer_t *buf)
{
    if (!buf) {
        return BUFFER_EINVAL;
    }
    
    if (buf->b_p)
        free(buf->b_p);
    
    buf->b_p = NULL;
    free(buf);

    return BUFFER_ENONE;
}

ssize_t buffer_wr(buffer_t *buf, unsigned char *data, ssize_t len)
{
    ssize_t i;
    
    if (!buf) {
        return BUFFER_EINVAL;
    }
    
    if (!data) {
        return BUFFER_EINVAL;
    }
    
    if (len < 1) {
        return BUFFER_EINVAL;
    }
    
    if (len > (buf->b_len - buf->b_num)) {
        return BUFFER_ENOSPC;
    }
    
    for (i = 0; i < len; i++) {
        if (buf->b_num == buf->b_len) {
            return BUFFER_ENOSPC;
        } else if (buf->b_num < buf->b_len) {
            buf->b_p[buf->b_idxw] = data[i];
            buf->b_num++;
        
            if (buf->b_idxw < (buf->b_len - 1))
                buf->b_idxw++;
            else
                buf->b_idxw = 0;
            
            if (buf->b_idxr == -1)
                buf->b_idxr = 0;
        } else {
            return BUFFER_EFAULT;
        }
    }
    
    return len;
}

ssize_t buffer_rd(buffer_t *buf, unsigned char *data, ssize_t len)
{
    ssize_t i;
    
    if (!buf) {
        return BUFFER_EINVAL;
    }
    
    if (!data) {
        return BUFFER_EINVAL;
    }
    
    if (len > buf->b_num) {
        return BUFFER_ERANGE;
    }
    
    for (i = 0; i < len; i++) {
        if (buf->b_num > 0) {
            data[i] = buf->b_p[buf->b_idxr];
            buf->b_num--;
            
            if (buf->b_idxr < (buf->b_len - 1))
                buf->b_idxr++;
            else
                buf->b_idxr = 0;
        } else {
            return BUFFER_EEMPTY;
        }
    }
    
    return len;
}

ssize_t buffer_get_len(buffer_t *buf)
{
    if (!buf) {
        return BUFFER_EINVAL;
    }
    
    return buf->b_len;
}

ssize_t buffer_get_num(buffer_t *buf)
{
    if (!buf) {
        return BUFFER_EINVAL;
    }
    
    return buf->b_num;
}

ssize_t buffer_get_free(buffer_t *buf)
{
    if (!buf) {
        return BUFFER_EINVAL;
    }
    
    return (buf->b_len - buf->b_num);
}
