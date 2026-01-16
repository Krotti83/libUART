===============================================
libUART API (Application Programming Interface)
===============================================

These document describes the application programming interface (API) for the ``libUART`` library
version ``0.2.0.0``.

Header ``UART.h``
=================

To use the library include the following header in your source code.

Usage
~~~~~

.. code-block:: c

    #include <UART.h>

Basic Functions
===============

Function ``UART_init()``
------------------------

Description
~~~~~~~~~~~
Initializes the ``UART`` library and creates an context.

Prototype
~~~~~~~~~

.. code-block:: c

    int UART_init(uart_ctx_t **ret_ctx);

Arguments
~~~~~~~~~
    - ``ret_ctx`` Pointer to Pointer to valid new context

Returns
~~~~~~~
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

Usage
~~~~~

.. code-block:: c

    #include <stdio.h>
    #include <UART.h>

    int ret;
    uart_ctx_t *ctx;

    ret = UART_init(&ctx);

    if (ret != UART_ESUCCESS) {
        printf("UART_init() failed\n");
    }

Function ``UART_free()``
------------------------

Description
~~~~~~~~~~~
Closes and frees all ``UART`` connections and destroys the context.

Prototype
~~~~~~~~~

.. code-block:: c

    int UART_free(uart_ctx_t *ctx);

Arguments
~~~~~~~~~
    - ``ctx`` Pointer to valid context

Returns
~~~~~~~
Returns ``UART_ESUCCESS`` on success, or an error code on failure. Possible error codes
are:

``UART_ECTX``:
``ctx`` is a ``NULL`` pointer.

``UART_ESYSAPI``:
Error in operating system specific API.

Usage
~~~~~

.. code-block:: c

    #include <stdio.h>
    #include <UART.h>

    int ret;
    uart_ctx_t *ctx;

    ret = UART_free(ctx);

    if (ret != UART_ESUCCESS) {
        printf("UART_free() failed\n");
    }

Function ``UART_get_device_list()``
-----------------------------------

Description
~~~~~~~~~~~
Return a list from all current available ``UART`` interfaces from the operating system.

Arguments
~~~~~~~~~
    - ``ctx`` Pointer to valid context
    - ``ret_uarts`` Pointer to buffer of ``UART`` interfaces, can be ``NULL``

Returns
~~~~~~~
Returns the number of available ``UART`` interfaces (the ``UART`` interfaces will be returned
in ``ret_uarts``). On failure an error code is returned. Possible error codes are:

``UART_ECTX``:
``ctx`` is a ``NULL`` pointer.

``UART_ESYSAPI``:
Error in operating system specific API.


Function ``UART_dev_open_name()``
---------------------------------

Description
~~~~~~~~~~~
Opens an ``UART`` interface by device name.

Prototype
~~~~~~~~~

.. code-block:: c

    uart_t *UART_dev_open_name(uart_ctx_t *ctx, const char *devname, enum e_baud baud, const char *opt);

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

Function ``UART_dev_open()``
----------------------------

Description
~~~~~~~~~~~
Opens an ``UART`` interface.

Prototype
~~~~~~~~~

.. code-block:: c

    int UART_dev_open(uart_ctx_t *ctx, uart_t *uart, enum e_baud baud, const char *opt);

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

Function ``UART_dev_close()``
-----------------------------

Description
~~~~~~~~~~~
Closes the ``UART`` interface.

Prototype
~~~~~~~~~

.. code-block:: c

    int UART_dev_close(uart_ctx_t *ctx, uart_t *uart);


Function ``UART_dev_free()``
----------------------------

Description
~~~~~~~~~~~
Frees the ``UART`` interface.

Prototype
~~~~~~~~~

.. code-block:: c

    int UART_dev_free(uart_ctx_t *ctx, uart_t *uart);


Basic Input/Output Functions
============================

This section contains low level function for sending and receiving from the ``UART`` interface.

Function ``UART_send()``
------------------------

Description
~~~~~~~~~~~
Sends data over the ``UART`` interface.

Prototype
~~~~~~~~~

.. code-block:: c

    ssize_t UART_send(uart_ctx_t *ctx, uart_t *uart, const void *send_buf, size_t len);


Function ``UART_recv()``
------------------------

Description
~~~~~~~~~~~
Receives data from the ``UART`` interface.

Prototype
~~~~~~~~~

.. code-block:: c

    ssize_t UART_recv(uart_ctx_t *ctx, uart_t *uart, void *recv_buf, size_t len);

Input/Output Functions
======================

This section contains higher level input and output functions like sending a string over
the ``UART`` interface, sending a formatted string, sending a single char or getting a char
from the ``UART`` interface. It also contains the setting and querying from the pin states.

Function ``UART_puts()``
------------------------

Description
~~~~~~~~~~~
Send a string over the ``UART`` interface.

Prototype
~~~~~~~~~

.. code-block:: c

    ssize_t UART_puts(uart_ctx_t *ctx, uart_t *uart, const char *msg);

Function ``UART_printf()``
--------------------------

Description
~~~~~~~~~~~
Send a formatted string over the ``UART`` interface. This function uses ``vsnprintf()`` internally
so all default formatting options should be supported. Currently the maximum string size is limited
to ``1024`` bytes.

Prototype
~~~~~~~~~

.. code-block:: c

    ssize_t UART_printf(uart_ctx_t *ctx, uart_t *uart, const char *fmt, ...);


Function ``UART_putc()``
------------------------

Description
~~~~~~~~~~~
Send a character over the ``UART`` interface.

Prototype
~~~~~~~~~

.. code-block:: c

    int UART_putc(uart_ctx_t *ctx, uart_t *uart, const char c);

Function ``UART_getc()``
------------------------

Description
~~~~~~~~~~~
Receive a character from the ``UART`` interface.

Prototype
~~~~~~~~~

.. code-block:: c

    int UART_getc(uart_ctx_t *ctx, uart_t *uart, char *ret_c);


Function ``UART_flush()``
-------------------------

Function ``UART_set_pin()``
---------------------------





Function ``UART_get_pin()``
---------------------------


Configuration Functions
=======================

This section contains the configurations functions from the ``UART`` interface like the baud rate,
the amount fo data bits, the amount of stop bits, the selected flow control and the parity.
It also contains useful functions like getting the device name and the underlying file descriptor
on ``Linux``/``FreeBSD`` or the file handle under Windows.

Function ``UART_set_baud()``
----------------------------


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


Function ``UART_get_baud()``
----------------------------

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


Function ``UART_set_databits()``
--------------------------------

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

Function ``UART_get_databits()``
--------------------------------

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

Function ``UART_set_parity()``
------------------------------

+------------------+--------+
| Enum             | Parity |
+==================+========+
| UART_PARITY_NONE | None   |
+------------------+--------+
| UART_PARITY_ODD  | Odd    |
+------------------+--------+
| UART_PARITY_EVEN | Even   |
+------------------+--------+

Function ``UART_get_parity()``
------------------------------

+------------------+--------+
| Enum             | Parity |
+==================+========+
| UART_PARITY_NONE | None   |
+------------------+--------+
| UART_PARITY_ODD  | Odd    |
+------------------+--------+
| UART_PARITY_EVEN | Even   |
+------------------+--------+

Function ``UART_set_stopbits()``
--------------------------------

+---------------+-------+------------------+
| Enum          | Value | Operating System |
+===============+=======+==================+
| UART_STOP_1_0 | 1     |                  |
+---------------+-------+------------------+
| UART_STOP_1_5 | 1.5   | Windows          |
+---------------+-------+------------------+
| UART_STOP_2_0 | 2     |                  |
+---------------+-------+------------------+

Function ``UART_get_stopbits()``
--------------------------------

+---------------+-------+------------------+
| Enum          | Value | Operating System |
+===============+=======+==================+
| UART_STOP_1_0 | 1     |                  |
+---------------+-------+------------------+
| UART_STOP_1_5 | 1.5   | Windows          |
+---------------+-------+------------------+
| UART_STOP_2_0 | 2     |                  |
+---------------+-------+------------------+

Function ``UART_set_flowctrl()``
--------------------------------

+--------------------+----------+
| Enum               | Flow     |
+====================+==========+
| UART_FLOW_NONE     | None     |
+--------------------+----------+
| UART_FLOW_SOFTWARE | Software |
+--------------------+----------+
| UART_FLOW_HARDWARE | Hardware |
+--------------------+----------+


Function ``UART_get_flowctrl()``
--------------------------------

+--------------------+----------+
| Enum               | Flow     |
+====================+==========+
| UART_FLOW_NONE     | None     |
+--------------------+----------+
| UART_FLOW_SOFTWARE | Software |
+--------------------+----------+
| UART_FLOW_HARDWARE | Hardware |
+--------------------+----------+

Function ``UART_get_fd()`` (Linux/FreeBSD only)
-----------------------------------------------

Function ``UART_get_handle()`` (Windows only)
---------------------------------------------

Function ``UART_get_dev()``
---------------------------

Miscellaneous Functions
=======================

This section contains various miscellaneous functions for error handling and other
useful functions. It also contain the version information functions from the library.

Function ``UART_get_libname()``
-------------------------------

Description
~~~~~~~~~~~
Returns the library name.

Prototype
~~~~~~~~~

.. code-block:: c

    char *UART_get_libname(void);

Arguments
~~~~~~~~~
None

Returns
~~~~~~~
Returns the library name string.

Usage
~~~~~

.. code-block:: c

    #include <stdio.h>
    #include <UART.h>

    printf("%s\n", UART_get_libname());

Function ``UART_get_libversion()``
----------------------------------

Description
~~~~~~~~~~~
Returns the library version string.

Prototype
~~~~~~~~~

.. code-block:: c

    char *UART_get_libversion(void);

Arguments
~~~~~~~~~
None

Returns
~~~~~~~
Returns the library version string.

Usage
~~~~~

.. code-block:: c

    #include <stdio.h>
    #include <UART.h>

    printf("%s\n", UART_get_libversion());
