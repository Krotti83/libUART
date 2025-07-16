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

#include <windows.h>

#include "error.h"
#include "version.h"
#include "util.h"

#include <UART.h>

/* COMMPROP structure */
/* COMMPROP.dwMaxBaud member */
#define WIN_BAUD_075                0x00000001
#define WIN_BAUD_110                0x00000002
#define WIN_BAUD_134_5              0x00000004
#define WIN_BAUD_150                0x00000008
#define WIN_BAUD_300                0x00000010
#define WIN_BAUD_600                0x00000020
#define WIN_BAUD_1200               0x00000040
#define WIN_BAUD_1800               0x00000080
#define WIN_BAUD_2400               0x00000100
#define WIN_BAUD_4800               0x00000200
#define WIN_BAUD_7200               0x00000400
#define WIN_BAUD_9600               0x00000800
#define WIN_BAUD_14400              0x00001000
#define WIN_BAUD_19200              0x00002000
#define WIN_BAUD_38400              0x00004000
#define WIN_BAUD_56K                0x00008000
#define WIN_BAUD_57600              0x00040000
#define WIN_BAUD_115200             0x00020000
#define WIN_BAUD_128K               0x00010000
#define WIN_BAUD_USER               0x10000000

/* COMMPROP.dwSettableParams member */
#define WIN_SP_BAUD                 0x0002
#define WIN_SP_DATABITS             0x0004
#define WIN_SP_HANDSHAKING          0x0010
#define WIN_SP_PARITY               0x0001
#define WIN_SP_PARITY_CHECK         0x0020
#define WIN_SP_STOPBITS             0x0008

/* COMMPROP.wSettableData member */
#define WIN_DATABITS_5              0x0001
#define WIN_DATABITS_6              0x0002
#define WIN_DATABITS_7              0x0004
#define WIN_DATABITS_8              0x0008
#define WIN_DATABITS_16             0x0010
#define WIN_DATABITS_16X            0x0020

/* COMMPROP.wSettableStopParity member */
#define WIN_STOPBITS_10             0x0001
#define WIN_STOPBITS_15             0x0002
#define WIN_STOPBITS_20             0x0004
#define WIN_PARITY_NONE             0x0100
#define WIN_PARITY_ODD              0x0200
#define WIN_PARITY_EVEN             0x0400

static int get_prop(struct _uart *uart)
{
    BOOL ret;
    
    ret = GetCommProperties(uart->h, &uart->prop);
    
    if (!ret) {
        uart->error = UART_ESYSTEM;
        _uart_error(uart, __func__, "GetCommProperties() failed");
        return -1;
    }
    
    return 0;
}

int _uart_baud_valid(int value)
{
    int E[] = {
        UART_BAUD_110,
        UART_BAUD_300,
        UART_BAUD_600,
        UART_BAUD_1200,
        UART_BAUD_2400,
        UART_BAUD_4800,
        UART_BAUD_9600,
        UART_BAUD_14400,
        UART_BAUD_19200,
        UART_BAUD_38400,
        UART_BAUD_57600,
        UART_BAUD_115200,
        UART_BAUD_128000,
        UART_BAUD_256000
    };
    
    if (enum_contains(E, sizeof(E)/sizeof(E[0]), value))
        return 1;
    
    return 0;
}

int _uart_init_baud(struct _uart *uart)
{
    BOOL ret;
    DCB dcb = { 0 };
    dcb.DCBlength = sizeof(dcb);

    ret = GetCommState(uart->h, &dcb);
    
    if (!ret) {
        uart->error = UART_ESYSTEM;
        _uart_error(uart, __func__, "GetCommState() failed");
        return -1;
    }
    
    if (!(uart->prop.dwSettableParams & SP_BAUD)) {
        uart->error = UART_EBAUD;
        _uart_error(uart, __func__, "Hardware/Driver doesn't support settable baud rate");
        return -1;
    }
    
    switch (uart->baud) {
    case UART_BAUD_110:
        dcb.BaudRate = CBR_110;
        break;
    case UART_BAUD_300:
        dcb.BaudRate = CBR_300;
        break;
    case UART_BAUD_600:
        dcb.BaudRate = CBR_600;
        break;
    case UART_BAUD_1200:
        dcb.BaudRate = CBR_1200;
        break;
    case UART_BAUD_2400:
        dcb.BaudRate = CBR_2400;
        break;
    case UART_BAUD_4800:
        dcb.BaudRate = CBR_4800;
        break;
    case UART_BAUD_9600:
        dcb.BaudRate = CBR_9600;
        break;
    case UART_BAUD_19200:
        dcb.BaudRate = CBR_19200;
        break;
    case UART_BAUD_38400:
        dcb.BaudRate = CBR_38400;
        break;
    case UART_BAUD_57600:
        dcb.BaudRate = CBR_57600;
        break;
    case UART_BAUD_115200:
        dcb.BaudRate = CBR_115200;
        break;
    case UART_BAUD_128000:
        dcb.BaudRate = CBR_128000;
        break;
    case UART_BAUD_256000:
        dcb.BaudRate = CBR_256000;
        break;
    default:
        uart->error = UART_EBAUD;
        _uart_error(uart, __func__, "invalid/unsupported baud rate");
        return -1;
    }
    
    ret = SetCommState(uart->h, &dcb);
    
    if (!ret) {
        uart->error = UART_ESYSTEM;
        _uart_error(uart, __func__, "SetCommState() failed");
        return -1;
    }
    
    uart->error = UART_ESUCCESS;

    return 0;
}

int _uart_init_databits(struct _uart *uart)
{
    BOOL ret;
    DCB dcb = { 0 };
    dcb.DCBlength = sizeof(dcb);
    
    ret = GetCommState(uart->h, &dcb);
    
    if (!ret) {
        uart->error = UART_ESYSTEM;
        _uart_error(uart, __func__, "GetCommState() failed");
        return -1;
    }
    
    if (!(uart->prop.dwSettableParams & WIN_SP_DATABITS)) {
        uart->error = UART_EDATA;
        _uart_error(uart, __func__, "Hardware/Driver doesn't support settable data bits length");
        return -1;
    }

    switch (uart->data_bits) {
    case UART_DATA_5:
        if (!(uart->prop.wSettableData & WIN_DATABITS_5)) {
            uart->error = UART_EDATA;
            _uart_error(uart, __func__, "Hardware/Driver doesn't support data bits length 5");
            return -1;
        }
        
        dcb.ByteSize = 5;
        break;
    case UART_DATA_6:
        if (!(uart->prop.wSettableData & WIN_DATABITS_6)) {
            uart->error = UART_EDATA;
            _uart_error(uart, __func__, "Hardware/Driver doesn't support data bits length 6");
            return -1;
        }
        
        dcb.ByteSize = 6;
        break;
    case UART_DATA_7:
        if (!(uart->prop.wSettableData & WIN_DATABITS_7)) {
            uart->error = UART_EDATA;
            _uart_error(uart, __func__, "Hardware/Driver doesn't support data bits length 7");
            return -1;
        }
        
        dcb.ByteSize = 7;
        break;
    case UART_DATA_8:
        if (!(uart->prop.wSettableData & WIN_DATABITS_8)) {
            uart->error = UART_EDATA;
            _uart_error(uart, __func__, "Hardware/Driver doesn't support data bits length 8");
            return -1;
        }
        
        dcb.ByteSize = 8;
        break;
    default:
        uart->error = UART_EDATA;
        _uart_error(uart, __func__, "invalid/unsupported data bits");
        return -1;
    }
    
    ret = SetCommState(uart->h, &dcb);
    
    if (!ret) {
        uart->error = UART_ESYSTEM;
        _uart_error(uart, __func__, "SetCommState() failed");
        return -1;
    }
    
    uart->error = UART_ESUCCESS;

    return 0;
}

int _uart_init_parity(struct _uart *uart)
{
    BOOL ret;
    DCB dcb = { 0 };
    dcb.DCBlength = sizeof(dcb);
    
    ret = GetCommState(uart->h, &dcb);
    
    if (!ret) {
        uart->error = UART_ESYSTEM;
        _uart_error(uart, __func__, "GetCommState() failed");
        return -1;
    }
    
    if (!(uart->prop.dwSettableParams & WIN_SP_PARITY)) {
        uart->error = UART_EPARITY;
        _uart_error(uart, __func__, "Hardware/Driver doesn't support settable/checkable parity");
        return -1;
    }
    
    switch (uart->parity) {
    case UART_PARITY_NONE:
        if (!(uart->prop.wSettableStopParity & WIN_PARITY_NONE)) {
            uart->error = UART_EPARITY;
            _uart_error(uart, __func__, "Hardware/Driver doesn't support none parity");
            return -1;
        }
        
        dcb.Parity = NOPARITY;
        break;
    case UART_PARITY_ODD:
        if (!(uart->prop.wSettableStopParity & WIN_PARITY_ODD)) {
            uart->error = UART_EPARITY;
            _uart_error(uart, __func__, "Hardware/Driver doesn't support odd parity");
            return -1;
        }
        
        dcb.Parity = ODDPARITY;
        break;
    case UART_PARITY_EVEN:
        if (!(uart->prop.wSettableStopParity & WIN_PARITY_EVEN)) {
            uart->error = UART_EPARITY;
            _uart_error(uart, __func__, "Hardware/Driver doesn't support even parity");
            return -1;
        }
        
        dcb.Parity = EVENPARITY;
        break;
    default:
        uart->error = UART_EPARITY;
        _uart_error(uart, __func__, "invalid/unsupported parity");
        return -1;
    }
    
    ret = SetCommState(uart->h, &dcb);
    
    if (!ret) {
        uart->error = UART_ESYSTEM;
        _uart_error(uart, __func__, "SetCommState() failed");
        return -1;
    }
    
    uart->error = UART_ESUCCESS;

    return 0;
}

int _uart_init_stopbits(struct _uart *uart)
{
    BOOL ret;
    DCB dcb = { 0 };
    dcb.DCBlength = sizeof(dcb);
    
    ret = GetCommState(uart->h, &dcb);
    
    if (!ret) {
        uart->error = UART_ESYSTEM;
        _uart_error(uart, __func__, "GetCommState() failed");
        return -1;
    }
    
    if (!(uart->prop.dwSettableParams & WIN_SP_DATABITS)) {
        uart->error = UART_ESTOP;
        _uart_error(uart, __func__, "Hardware/Driver doesn't support settable stop bits length");
        return -1;
    }
    
    switch (uart->stop_bits) {
    case UART_STOP_1_0:
        if (!(uart->prop.wSettableStopParity & WIN_STOPBITS_10)) {
            uart->error = UART_ESTOP;
            _uart_error(uart, __func__, "Hardware/Driver doesn't support stop bits length 1");
            return -1;
        }
        
        dcb.StopBits = ONESTOPBIT;
        break;
    case UART_STOP_2_0:
        if (!(uart->prop.wSettableStopParity & WIN_STOPBITS_20)) {
            uart->error = UART_ESTOP;
            _uart_error(uart, __func__, "Hardware/Driver doesn't support stop bits length 2");
            return -1;
        }
        
        dcb.StopBits = TWOSTOPBITS;
        break;
    default:
        uart->error = UART_ESTOP;
        _uart_error(uart, __func__, "invalid/unsupported stop bits");
        return -1;
    }
    
    ret = SetCommState(uart->h, &dcb);
    
    if (!ret) {
        uart->error = UART_ESYSTEM;
        _uart_error(uart, __func__, "SetCommState() failed");
        return -1;
    }
    
    uart->error = UART_ESUCCESS;

    return 0;
}

int _uart_init_flow(struct _uart *uart)
{
    BOOL ret;
    DCB dcb = { 0 };
    dcb.DCBlength = sizeof(dcb);
    
    ret =  GetCommState(uart->h, &dcb);
    
    if (!ret) {
        uart->error = UART_ESYSTEM;
        _uart_error(uart, __func__, "GetCommState() failed");
        return -1;
    }
    
    switch (uart->flow_ctrl) {
    case UART_FLOW_NO:
        dcb.fOutxCtsFlow = FALSE,
        dcb.fOutxDsrFlow = FALSE,
        dcb.fDtrControl = DTR_CONTROL_DISABLE;
        dcb.fRtsControl = RTS_CONTROL_DISABLE;
        dcb.fOutX = FALSE;
        dcb.fInX = FALSE;
        break;
    case UART_FLOW_SW:
        dcb.fOutxCtsFlow = FALSE,
        dcb.fOutxDsrFlow = FALSE,
        dcb.fDtrControl = DTR_CONTROL_DISABLE;
        dcb.fRtsControl = RTS_CONTROL_DISABLE;
        dcb.fOutX = TRUE;
        dcb.fInX = TRUE;
        break;
    case UART_FLOW_HW:
        dcb.fOutxCtsFlow = TRUE,
        dcb.fOutxDsrFlow = TRUE,
        dcb.fDtrControl = DTR_CONTROL_ENABLE;
        dcb.fRtsControl = RTS_CONTROL_ENABLE;
        dcb.fOutX = FALSE;
        dcb.fInX = FALSE;
        break;
    default:
        uart->error = UART_EFLOW;
        _uart_error(uart, __func__, "invalid/unsupported flow control");
        return -1;
    }
    
    ret =  SetCommState(uart->h, &dcb);
    
    if (!ret) {
        uart->error = UART_ESYSTEM;
        _uart_error(uart, __func__, "SetCommState() failed");
        return -1;
    }
    
    uart->error = UART_ESUCCESS;

    return 0;
}

int _uart_init(void)
{
    return UART_ESUCCESS;
}

int _uart_open(struct _uart *uart)
{
    int ret;
    HANDLE h;
    
    h = CreateFile((LPCTSTR) uart->dev,
                   GENERIC_READ | GENERIC_WRITE,
                   0,
                   NULL,
                   OPEN_EXISTING,
                   0,
                   NULL);
    
    if (h == INVALID_HANDLE_VALUE) {
        uart->error = UART_ESYSTEM;
        _uart_error(uart, __func__, "CreateFile() failed");
        return -1;
    }
    
    uart->h = h;
    
    ret = get_prop(uart);
    
    if (ret == -1) {
        CloseHandle(h);
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
    
    if (ret == -1) {
        CloseHandle(h);
        return -1;
    }
    
    uart->error = UART_ESUCCESS;

    return 0;
}

void _uart_close(struct _uart *uart)
{
    CloseHandle(uart->h);
}

ssize_t _uart_send(struct _uart *uart, char *send_buf, size_t len)
{
    int ret;
    DWORD dwbytestowrite = (DWORD) len;
    DWORD dwbyteswritten;
    
    ret = WriteFile(uart->h, 
                    (LPVOID) send_buf, 
                    dwbytestowrite, 
                    &dwbyteswritten, 
                    NULL);
    
    if (!ret) {
        uart->error = UART_ESYSTEM;
        _uart_error(uart, __func__, "WriteFile() failed");
        return -1;
    }
    
    ret = (int) dwbyteswritten;
    
    if (ret != (int) len) {
        uart->error = UART_ESYSTEM;
        _uart_error(uart, __func__, "WriteFile() could not write all data");
        return (ssize_t) ret;
    }
    
    uart->error = UART_ESUCCESS;

    return (ssize_t) ret;
}

ssize_t _uart_recv(struct _uart *uart, char *recv_buf, size_t len)
{
    int ret = 0;
    DWORD dwbytesread = (DWORD) len;
    
    ret = ReadFile(uart->h, 
                   (LPVOID) recv_buf, 
                   (DWORD) len, 
                   &dwbytesread, 
                   NULL);
    
    if (!ret) {
        uart->error = UART_ESYSTEM;
        _uart_error(uart, __func__, "ReadFile() failed");
        return -1;
    }
    
    ret = (int) dwbytesread;
    uart->error = UART_ESUCCESS;

    return (ssize_t) ret;
}

int _uart_flush(struct _uart *uart)
{
    int ret;
    
    ret = FlushFileBuffers(uart->h);
    
    if (!ret) {
        uart->error = UART_ESYSTEM;
        _uart_error(uart, __func__, "FlushFileBuffers() failed");
        return -1;
    }

    uart->error = UART_ESUCCESS;
    return 0;
}

int _uart_set_pin(struct _uart *uart, enum e_pins pin, int state)
{
    int ret;
    DWORD dwfunc;
    
    switch (pin) {
    case UART_PIN_RTS:
        if (state == UART_PIN_HIGH)
            dwfunc = SETRTS;
        else
            dwfunc = CLRRTS;
        break;
    case UART_PIN_DTR:
        if (state == UART_PIN_HIGH)
            dwfunc = SETDTR;
        else
            dwfunc = CLRDTR;
        break;
    default:
        uart->error = UART_EPIN;
        _uart_error(uart, __func__, "invalid pin");
        return -1;
    }
    
    ret = EscapeCommFunction(uart->h, dwfunc);
    
    if (!ret) {
        uart->error = UART_ESYSTEM;
        _uart_error(uart, __func__, "EscapeCommFunction() failed");
        return -1;
    }

    uart->error = UART_ESUCCESS;
    return 0;
}

int _uart_get_pin(struct _uart *uart, enum e_pins pin, int *state)
{
    int ret = 0;
    DWORD dwmstat;

    ret = GetCommModemStatus(uart->h, &dwmstat);

    if (!ret) {
        uart->error = UART_ESYSTEM;
        _uart_error(uart, __func__, "GetCommModemStatus() failed");
        return -1;
    }
    
    switch (pin) {
    case UART_PIN_CTS:
        if (dwmstat == MS_CTS_ON)
            ret = UART_PIN_HIGH;
        else
            ret = UART_PIN_LOW;
        break;
    case UART_PIN_DSR:
        if (dwmstat == MS_DSR_ON)
            ret = UART_PIN_HIGH;
        else
            ret = UART_PIN_LOW;
        break;
    case UART_PIN_DCD:
        if (dwmstat == MS_RLSD_ON)
            ret = UART_PIN_HIGH;
        else
            ret = UART_PIN_LOW;
        break;
    case UART_PIN_RI:
        if (dwmstat == MS_RING_ON)
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
    COMSTAT comst;
    DWORD dwerror;
    
    ret = ClearCommError(uart->h, &dwerror, &comst);
    
    if (!ret) {
        uart->error = UART_ESYSTEM;
        _uart_error(uart, __func__, "ClearCommError() failed");
        return -1;
    }
    
    ret = (int) comst.cbInQue;
    (*bytes) = ret;
    uart->error = UART_ESUCCESS;

    return 0;
}
