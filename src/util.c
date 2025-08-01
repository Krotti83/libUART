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
#include <string.h>

int enum_contains(int enum_values[], int len, int value)
{
    int i;
    
    for (i = 0; i < len; i++)
        if (enum_values[i] == value)
            return 1;
    
    return 0;
}

int strnrcmp(const char *str1, const char *str2, size_t num)
{
    size_t str1_len;
    size_t str2_len;
    char *p_s1;
    char *p_s2;
    size_t i;

    str1_len = strlen(str1);
    str2_len = strlen(str2);

    if (str1_len < num) {
        return 1;
    }

    if (str2_len < num) {
        return 1;
    }

    p_s1 = (char *) str1;
    p_s1 += str1_len;
    p_s2 = (char *) str2;
    p_s2 += str2_len;

    for (i = 0; i < num; i++) {
        if (*p_s1 != *p_s2) {
            return 1;
        }

        p_s1--;
        p_s2--;
    }

    return 0;
}
