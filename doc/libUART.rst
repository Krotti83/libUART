===========================================
libUART v0.2.0.0 - Easy to use UART Library
===========================================

1 - Preamble
------------

Easy to use library for accessing the **UART** (serial interface). The library use the **POSIX**
functions from ``termios`` on **Linux** and **FreeBSD**. So it should also be possible to use the
library on other ***BSD**. systems too, but currently not tested. In the ``UART_open()`` function
the library setups the **UART** interface in *raw* and *none-blocking* mode currently on **POSIX**
compatible systems like **Linux** and **FreeBSD**. On **Windows** the **UART** library doesn't use
a *none-blocking* mode, because it's not supported by the used function ``CreateFile()``. The
library function ``UART_init()`` tests if the user have sufficient permissions to use the **UART**
interfaces on **Linux** and **FreeBSD**. For **Linux** systems the user which uses the library
should be member from group ``dialout`` and on **FreeBSD** from group ``dialer``, otherwise the
``UART_init()`` fails with the error ``UART_EPERM``.

2 - License
-----------

    Copyright (c) 2025, 2026 Johannes Krottmayer <krotti83@proton.me>

    Permission to use, copy, modify, and/or distribute this software for any
    purpose with or without fee is hereby granted, provided that the above
    copyright notice and this permission notice appear in all copies.

    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
    WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
    MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
    ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

3 - Application Programming Interface - Header ``UART.h``
---------------------------------------------------------

To use the library include the following header in your source code.

Usage
"""""

.. rstcheck: ignore-next-code-block
.. code-block:: c

    #include <UART.h>

.. rstcheck: ignore-languages=c

4 - Application Programming Interface - Basic Functions
-------------------------------------------------------


asdasdjfaksdfs

4.1 - Function ``UART_init()``
------------------------------

4.1.1 - Description
"""""""""""""""""""

Initializes the ``UART`` library and creates an context.

4.1.2 - Prototype
"""""""""""""""""

.. rstcheck: ignore-next-code-block
.. code-block:: c

    int UART_init(uart_ctx_t **ret_ctx);

.. rstcheck: ignore-languages=c

4.1.3 - Arguments
"""""""""""""""""

    - ``ret_ctx`` Pointer to Pointer to valid new context

4.1.4 - Returns
"""""""""""""""

Returns ``UART_ESUCCESS`` on success, or an error code on failure. Possible error codes
are:

``UART_EINVAL``:
``ret_ctx`` is a ``NULL`` pointer.

``UART_ENOMEM``:
No free memory available.

``UART_ESYSAPI``:
Error in operating system specific API.

``UART_EPERM``:
Insufficient permissions (currently only on ``Linux``/``FreeBSD``)

4.1.5 - Usage
"""""""""""""

.. rstcheck: ignore-next-code-block
.. code-block:: c

    #include <stdio.h>
    #include <UART.h>

    int ret;
    uart_ctx_t *ctx;

    ret = UART_init(&ctx);

    if (ret != UART_ESUCCESS) {
        printf("UART_init() failed\n");
    }

.. rstcheck: ignore-languages=c

4.2 - Function ``UART_free()``
------------------------------

4.2.1 - Description
"""""""""""""""""""

Closes and frees all ``UART`` connections and destroys the context.

4.2.2 - Prototype
"""""""""""""""""

.. rstcheck: ignore-next-code-block
.. code-block:: c

    int UART_free(uart_ctx_t *ctx);

.. rstcheck: ignore-languages=c

4.2.3 - Arguments
"""""""""""""""""

    - ``ctx`` Pointer to valid context

4.2.4 - Returns
"""""""""""""""

Returns ``UART_ESUCCESS`` on success, or an error code on failure. Possible error codes
are:

``UART_ECTX``:
``ctx`` is a ``NULL`` pointer.

``UART_ESYSAPI``:
Error in operating system specific API.

4.2.5 - Usage
"""""""""""""

.. rstcheck: ignore-next-code-block
.. code-block:: c

    #include <stdio.h>
    #include <UART.h>

    int ret;
    uart_ctx_t *ctx;

    ret = UART_free(ctx);

    if (ret != UART_ESUCCESS) {
        printf("UART_free() failed\n");
    }

.. rstcheck: ignore-languages=c

4.3 - Function ``UART_get_device_list()``
-----------------------------------------

4.3.1 - Description
"""""""""""""""""""

Return a list from all current available ``UART`` interfaces from the operating system.

4.3.2 - Prototype
"""""""""""""""""

.. rstcheck: ignore-next-code-block
.. code-block:: c

    ssize_t UART_get_device_list(uart_ctx_t *ctx, uart_t **ret_uarts, size_t num);

.. rstcheck: ignore-languages=c

4.3.3 - Arguments
"""""""""""""""""

    - ``ctx`` Pointer to valid context
    - ``ret_uarts`` Pointer to buffer of ``UART`` interfaces, can be ``NULL``

4.3.4 - Returns
"""""""""""""""

Returns the number of available ``UART`` interfaces (the ``UART`` interfaces will be returned
in ``ret_uarts``). On failure an error code is returned. Possible error codes are:

``UART_ECTX``:
``ctx`` is a ``NULL`` pointer.

``UART_ESYSAPI``:
Error in operating system specific API.


4.4 - Function ``UART_dev_open_name()``
---------------------------------------

4.4.1 - Description
"""""""""""""""""""

Opens an ``UART`` interface by device name.

4.4.2 - Prototype
"""""""""""""""""

.. rstcheck: ignore-next-code-block
.. code-block:: c

    uart_t *UART_dev_open_name(uart_ctx_t *ctx, const char *devname, enum e_baud baud, const char *opt);

.. rstcheck: ignore-languages=c

+-------------------+-----------+------------------+
| Enum              | Baud Rate | Operating System |
+===================+===========+==================+
| UART_BAUD_0       | 0         | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_50      | 50        | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_75      | 75        | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_110     | 110       |                  |
+-------------------+-----------+------------------+
| UART_BAUD_134     | 134       | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_150     | 150       | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_200     | 200       | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_300     | 300       |                  |
+-------------------+-----------+------------------+
| UART_BAUD_600     | 600       |                  |
+-------------------+-----------+------------------+
| UART_BAUD_1200    | 1200      |                  |
+-------------------+-----------+------------------+
| UART_BAUD_1800    | 1800      | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_2400    | 2400      |                  |
+-------------------+-----------+------------------+
| UART_BAUD_4800    | 4800      |                  |
+-------------------+-----------+------------------+
| UART_BAUD_9600    | 9600      |                  |
+-------------------+-----------+------------------+
| UART_BAUD_14400   | 14400     | Windows          |
+-------------------+-----------+------------------+
| UART_BAUD_19200   | 19200     |                  |
+-------------------+-----------+------------------+
| UART_BAUD_38400   | 38400     |                  |
+-------------------+-----------+------------------+
| UART_BAUD_57600   | 57600     |                  |
+-------------------+-----------+------------------+
| UART_BAUD_115200  | 115200    |                  |
+-------------------+-----------+------------------+
| UART_BAUD_128000  | 128000    | Windows          |
+-------------------+-----------+------------------+
| UART_BAUD_256000  | 256000    | Windows          |
+-------------------+-----------+------------------+
| UART_BAUD_230400  | 230400    | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_460800  | 460800    | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_500000  | 500000    | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_576000  | 576000    | Linux            |
+-------------------+-----------+------------------+
| UART_BAUD_921600  | 921600    | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_1000000 | 1000000   | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_1152000 | 1152000   | Linux            |
+-------------------+-----------+------------------+
| UART_BAUD_1500000 | 1500000   | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_2000000 | 2000000   | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_2500000 | 2500000   | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_3000000 | 3000000   | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_3500000 | 3500000   | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_4000000 | 4000000   | Linux/FreeBSD    |
+-------------------+-----------+------------------+

4.5 - Function ``UART_dev_open()``
----------------------------------

4.5.1 - Description
"""""""""""""""""""

Opens an ``UART`` interface.

4.5.2 - Prototype
"""""""""""""""""

.. rstcheck: ignore-next-code-block
.. code-block:: c

    int UART_dev_open(uart_ctx_t *ctx, uart_t *uart, enum e_baud baud, const char *opt);

.. rstcheck: ignore-languages=c

+-------------------+-----------+------------------+
| Enum              | Baud Rate | Operating System |
+===================+===========+==================+
| UART_BAUD_0       | 0         | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_50      | 50        | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_75      | 75        | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_110     | 110       |                  |
+-------------------+-----------+------------------+
| UART_BAUD_134     | 134       | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_150     | 150       | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_200     | 200       | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_300     | 300       |                  |
+-------------------+-----------+------------------+
| UART_BAUD_600     | 600       |                  |
+-------------------+-----------+------------------+
| UART_BAUD_1200    | 1200      |                  |
+-------------------+-----------+------------------+
| UART_BAUD_1800    | 1800      | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_2400    | 2400      |                  |
+-------------------+-----------+------------------+
| UART_BAUD_4800    | 4800      |                  |
+-------------------+-----------+------------------+
| UART_BAUD_9600    | 9600      |                  |
+-------------------+-----------+------------------+
| UART_BAUD_14400   | 14400     | Windows          |
+-------------------+-----------+------------------+
| UART_BAUD_19200   | 19200     |                  |
+-------------------+-----------+------------------+
| UART_BAUD_38400   | 38400     |                  |
+-------------------+-----------+------------------+
| UART_BAUD_57600   | 57600     |                  |
+-------------------+-----------+------------------+
| UART_BAUD_115200  | 115200    |                  |
+-------------------+-----------+------------------+
| UART_BAUD_128000  | 128000    | Windows          |
+-------------------+-----------+------------------+
| UART_BAUD_256000  | 256000    | Windows          |
+-------------------+-----------+------------------+
| UART_BAUD_230400  | 230400    | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_460800  | 460800    | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_500000  | 500000    | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_576000  | 576000    | Linux            |
+-------------------+-----------+------------------+
| UART_BAUD_921600  | 921600    | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_1000000 | 1000000   | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_1152000 | 1152000   | Linux            |
+-------------------+-----------+------------------+
| UART_BAUD_1500000 | 1500000   | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_2000000 | 2000000   | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_2500000 | 2500000   | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_3000000 | 3000000   | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_3500000 | 3500000   | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_4000000 | 4000000   | Linux/FreeBSD    |
+-------------------+-----------+------------------+

4.6 - Function ``UART_dev_close()``
-----------------------------------

4.6.1 - Description
"""""""""""""""""""

Closes the ``UART`` interface.

4.6.2 - Prototype
"""""""""""""""""

.. rstcheck: ignore-next-code-block
.. code-block:: c

    int UART_dev_close(uart_ctx_t *ctx, uart_t *uart);

.. rstcheck: ignore-languages=c


4.7 - Function ``UART_dev_free()``
----------------------------------

4.7.1 - Description
"""""""""""""""""""

Frees the ``UART`` interface.

4.7.2 - Prototype
"""""""""""""""""

.. rstcheck: ignore-next-code-block
.. code-block:: c

    int UART_dev_free(uart_ctx_t *ctx, uart_t *uart);

.. rstcheck: ignore-languages=c

5 - Application Programming Interface - Basic Input/Output Functions
--------------------------------------------------------------------

This section contains low level function for sending and receiving from the ``UART`` interface.

5.1 - Function ``UART_send()``
------------------------------

5.1.1 - Description
"""""""""""""""""""

Sends data over the ``UART`` interface.

5.1.2 - Prototype
"""""""""""""""""

.. rstcheck: ignore-next-code-block
.. code-block:: c

    ssize_t UART_send(uart_ctx_t *ctx, uart_t *uart, const void *send_buf, size_t len);

.. rstcheck: ignore-languages=c


5.2 - Function ``UART_recv()``
------------------------------

5.2.1 - Description
"""""""""""""""""""

Receives data from the ``UART`` interface.

5.2.2 - Prototype
"""""""""""""""""

.. rstcheck: ignore-next-code-block
.. code-block:: c

    ssize_t UART_recv(uart_ctx_t *ctx, uart_t *uart, void *recv_buf, size_t len);

.. rstcheck: ignore-languages=c


6 - Application Programming Interface - Input/Output Functions
--------------------------------------------------------------

This section contains higher level input and output functions like sending a string over
the ``UART`` interface, sending a formatted string, sending a single char or getting a char
from the ``UART`` interface. It also contains the setting and querying from the pin states.

6.1 - Function ``UART_puts()``
------------------------------

6.1.1 - Description
"""""""""""""""""""

Send a string over the ``UART`` interface.

6.1.2 - Prototype
"""""""""""""""""

.. rstcheck: ignore-next-code-block
.. code-block:: c

    ssize_t UART_puts(uart_ctx_t *ctx, uart_t *uart, const char *msg);

.. rstcheck: ignore-languages=c

6.2 - Function ``UART_printf()``
--------------------------------

6.2.1 - Description
"""""""""""""""""""

Send a formatted string over the ``UART`` interface. This function uses ``vsnprintf()`` internally
so all default formatting options should be supported. Currently the maximum string size is limited
to ``1024`` bytes.

6.2.2 - Prototype
"""""""""""""""""

.. rstcheck: ignore-next-code-block
.. code-block:: c

    ssize_t UART_printf(uart_ctx_t *ctx, uart_t *uart, const char *fmt, ...);

.. rstcheck: ignore-languages=c


6.3 - Function ``UART_putc()``
------------------------------

6.3.1 - Description
"""""""""""""""""""

Send a character over the ``UART`` interface.

6.3.2 - Prototype
"""""""""""""""""

.. rstcheck: ignore-next-code-block
.. code-block:: c

    int UART_putc(uart_ctx_t *ctx, uart_t *uart, const char c);

.. rstcheck: ignore-languages=c

6.4 - Function ``UART_getc()``
------------------------------

6.4.1 - Description
"""""""""""""""""""

Receive a character from the ``UART`` interface.

6.4.2 - Prototype
"""""""""""""""""

.. rstcheck: ignore-next-code-block
.. code-block:: c

    int UART_getc(uart_ctx_t *ctx, uart_t *uart, char *ret_c);

.. rstcheck: ignore-languages=c


6.5 - Function ``UART_flush()``
-------------------------------

6.6 - Function ``UART_set_pin()``
---------------------------------





6.7 - Function ``UART_get_pin()``
---------------------------------


7 - Application Programming Interface - Configuration Functions
---------------------------------------------------------------

This section contains the configurations functions from the ``UART`` interface like the baud rate,
the amount fo data bits, the amount of stop bits, the selected flow control and the parity.
It also contains useful functions like getting the device name and the underlying file descriptor
on ``Linux``/``FreeBSD`` or the file handle under Windows.

7.1 - Function ``UART_set_baud()``
----------------------------------


+-------------------+-----------+------------------+
| Enum              | Baud Rate | Operating System |
+===================+===========+==================+
| UART_BAUD_0       | 0         | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_50      | 50        | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_75      | 75        | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_110     | 110       |                  |
+-------------------+-----------+------------------+
| UART_BAUD_134     | 134       | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_150     | 150       | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_200     | 200       | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_300     | 300       |                  |
+-------------------+-----------+------------------+
| UART_BAUD_600     | 600       |                  |
+-------------------+-----------+------------------+
| UART_BAUD_1200    | 1200      |                  |
+-------------------+-----------+------------------+
| UART_BAUD_1800    | 1800      | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_2400    | 2400      |                  |
+-------------------+-----------+------------------+
| UART_BAUD_4800    | 4800      |                  |
+-------------------+-----------+------------------+
| UART_BAUD_9600    | 9600      |                  |
+-------------------+-----------+------------------+
| UART_BAUD_14400   | 14400     | Windows          |
+-------------------+-----------+------------------+
| UART_BAUD_19200   | 19200     |                  |
+-------------------+-----------+------------------+
| UART_BAUD_38400   | 38400     |                  |
+-------------------+-----------+------------------+
| UART_BAUD_57600   | 57600     |                  |
+-------------------+-----------+------------------+
| UART_BAUD_115200  | 115200    |                  |
+-------------------+-----------+------------------+
| UART_BAUD_128000  | 128000    | Windows          |
+-------------------+-----------+------------------+
| UART_BAUD_256000  | 256000    | Windows          |
+-------------------+-----------+------------------+
| UART_BAUD_230400  | 230400    | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_460800  | 460800    | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_500000  | 500000    | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_576000  | 576000    | Linux            |
+-------------------+-----------+------------------+
| UART_BAUD_921600  | 921600    | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_1000000 | 1000000   | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_1152000 | 1152000   | Linux            |
+-------------------+-----------+------------------+
| UART_BAUD_1500000 | 1500000   | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_2000000 | 2000000   | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_2500000 | 2500000   | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_3000000 | 3000000   | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_3500000 | 3500000   | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_4000000 | 4000000   | Linux/FreeBSD    |
+-------------------+-----------+------------------+


7.2 - Function ``UART_get_baud()``
----------------------------------

+-------------------+-----------+------------------+
| Enum              | Baud Rate | Operating System |
+===================+===========+==================+
| UART_BAUD_0       | 0         | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_50      | 50        | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_75      | 75        | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_110     | 110       |                  |
+-------------------+-----------+------------------+
| UART_BAUD_134     | 134       | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_150     | 150       | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_200     | 200       | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_300     | 300       |                  |
+-------------------+-----------+------------------+
| UART_BAUD_600     | 600       |                  |
+-------------------+-----------+------------------+
| UART_BAUD_1200    | 1200      |                  |
+-------------------+-----------+------------------+
| UART_BAUD_1800    | 1800      | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_2400    | 2400      |                  |
+-------------------+-----------+------------------+
| UART_BAUD_4800    | 4800      |                  |
+-------------------+-----------+------------------+
| UART_BAUD_9600    | 9600      |                  |
+-------------------+-----------+------------------+
| UART_BAUD_14400   | 14400     | Windows          |
+-------------------+-----------+------------------+
| UART_BAUD_19200   | 19200     |                  |
+-------------------+-----------+------------------+
| UART_BAUD_38400   | 38400     |                  |
+-------------------+-----------+------------------+
| UART_BAUD_57600   | 57600     |                  |
+-------------------+-----------+------------------+
| UART_BAUD_115200  | 115200    |                  |
+-------------------+-----------+------------------+
| UART_BAUD_128000  | 128000    | Windows          |
+-------------------+-----------+------------------+
| UART_BAUD_256000  | 256000    | Windows          |
+-------------------+-----------+------------------+
| UART_BAUD_230400  | 230400    | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_460800  | 460800    | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_500000  | 500000    | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_576000  | 576000    | Linux            |
+-------------------+-----------+------------------+
| UART_BAUD_921600  | 921600    | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_1000000 | 1000000   | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_1152000 | 1152000   | Linux            |
+-------------------+-----------+------------------+
| UART_BAUD_1500000 | 1500000   | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_2000000 | 2000000   | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_2500000 | 2500000   | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_3000000 | 3000000   | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_3500000 | 3500000   | Linux/FreeBSD    |
+-------------------+-----------+------------------+
| UART_BAUD_4000000 | 4000000   | Linux/FreeBSD    |
+-------------------+-----------+------------------+


7.3 - Function ``UART_set_databits()``
--------------------------------------

+--------------+-------+------------------+
| Enum         | Value | Operating System |
+==============+=======+==================+
| UART_DATA_5  | 5     |                  |
+--------------+-------+------------------+
| UART_DATA_6  | 6     |                  |
+--------------+-------+------------------+
| UART_DATA_7  | 7     |                  |
+--------------+-------+------------------+
| UART_DATA_8  | 8     |                  |
+--------------+-------+------------------+
| UART_DATA_16 | 16    | Windows          |
+--------------+-------+------------------+

7.4 - Function ``UART_get_databits()``
--------------------------------------

+--------------+-------+------------------+
| Enum         | Value | Operating System |
+==============+=======+==================+
| UART_DATA_5  | 5     |                  |
+--------------+-------+------------------+
| UART_DATA_6  | 6     |                  |
+--------------+-------+------------------+
| UART_DATA_7  | 7     |                  |
+--------------+-------+------------------+
| UART_DATA_8  | 8     |                  |
+--------------+-------+------------------+
| UART_DATA_16 | 16    | Windows          |
+--------------+-------+------------------+

7.5 - Function ``UART_set_parity()``
------------------------------------

+------------------+--------+
| Enum             | Parity |
+==================+========+
| UART_PARITY_NONE | None   |
+------------------+--------+
| UART_PARITY_ODD  | Odd    |
+------------------+--------+
| UART_PARITY_EVEN | Even   |
+------------------+--------+

7.6 - Function ``UART_get_parity()``
------------------------------------

+------------------+--------+
| Enum             | Parity |
+==================+========+
| UART_PARITY_NONE | None   |
+------------------+--------+
| UART_PARITY_ODD  | Odd    |
+------------------+--------+
| UART_PARITY_EVEN | Even   |
+------------------+--------+

7.7 - Function ``UART_set_stopbits()``
--------------------------------------

+---------------+-------+------------------+
| Enum          | Value | Operating System |
+===============+=======+==================+
| UART_STOP_1_0 | 1     |                  |
+---------------+-------+------------------+
| UART_STOP_1_5 | 1.5   | Windows          |
+---------------+-------+------------------+
| UART_STOP_2_0 | 2     |                  |
+---------------+-------+------------------+

7.8 - Function ``UART_get_stopbits()``
--------------------------------------

+---------------+-------+------------------+
| Enum          | Value | Operating System |
+===============+=======+==================+
| UART_STOP_1_0 | 1     |                  |
+---------------+-------+------------------+
| UART_STOP_1_5 | 1.5   | Windows          |
+---------------+-------+------------------+
| UART_STOP_2_0 | 2     |                  |
+---------------+-------+------------------+

7.9 - Function ``UART_set_flowctrl()``
--------------------------------------

+--------------------+----------+
| Enum               | Flow     |
+====================+==========+
| UART_FLOW_NONE     | None     |
+--------------------+----------+
| UART_FLOW_SOFTWARE | Software |
+--------------------+----------+
| UART_FLOW_HARDWARE | Hardware |
+--------------------+----------+


7.10 - Function ``UART_get_flowctrl()``
---------------------------------------

+--------------------+----------+
| Enum               | Flow     |
+====================+==========+
| UART_FLOW_NONE     | None     |
+--------------------+----------+
| UART_FLOW_SOFTWARE | Software |
+--------------------+----------+
| UART_FLOW_HARDWARE | Hardware |
+--------------------+----------+

7.11 - Function ``UART_get_fd()`` (Linux/FreeBSD only)
------------------------------------------------------

7.12 - Function ``UART_get_handle()`` (Windows only)
----------------------------------------------------

7.13 - Function ``UART_get_dev()``
----------------------------------

8 - Application Programming Interface - Miscellaneous Functions
---------------------------------------------------------------

This section contains various miscellaneous functions for error handling and other
useful functions. It also contain the version information functions from the library.

8.8 - Function ``UART_get_libname()``
-------------------------------------

8.8.1 - Description
"""""""""""""""""""

Returns the library name.

8.8.2 - Prototype
"""""""""""""""""

.. rstcheck: ignore-next-code-block
.. code-block:: c

    char *UART_get_libname(void);

.. rstcheck: ignore-languages=c

8.8.3 - Arguments
"""""""""""""""""

None

8.8.4 - Returns
"""""""""""""""

Returns the library name string.

8.8.5 - Usage
"""""""""""""

.. rstcheck: ignore-next-code-block
.. code-block:: c

    #include <stdio.h>
    #include <UART.h>

    printf("%s\n", UART_get_libname());

.. rstcheck: ignore-languages=c

8.9 - Function ``UART_get_libversion()``
----------------------------------------

8.9.1 - Description
"""""""""""""""""""

Returns the library version string.

8.9.2 - Prototype
"""""""""""""""""

.. rstcheck: ignore-next-code-block
.. code-block:: c

    char *UART_get_libversion(void);

.. rstcheck: ignore-languages=c

8.9.3 - Arguments
"""""""""""""""""

None

8.9.4 - Returns
""""""""""""""""

Returns the library version string.

8.9.5 - Usage
"""""""""""""

.. rstcheck: ignore-next-code-block
.. code-block:: c

    #include <stdio.h>
    #include <UART.h>

    printf("%s\n", UART_get_libversion());

.. rstcheck: ignore-languages=c
