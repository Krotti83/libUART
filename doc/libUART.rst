===============================================
libUART API (Application Programming Interface)
===============================================

These document describes the application programming interface (API) for the ``libUART`` library.

Header ``libUART``
------------------

To use the library include the following header in your source code.

Usage
~~~~~

.. code-block:: c

    #include <UART.h>

Function ``int UART_init(void)``
---------------------------------

Description
~~~~~~~~~~~
Initializes the UART library.

Arguments
~~~~~~~~~
    - None

Returns
~~~~~~~
Returns ``0`` on success, or an error code on failure.

Usage
~~~~~

.. code-block:: c

    UART_init();

Notes
~~~~~

This function is currently a stub. It isn't really required, but should
be used for further API changes.

Function ``uart_t *UART_open(const char *dev, enum e_baud baud, const char *opt)``
----------------------------------------------------------------------------------

Description
~~~~~~~~~~~
Opens the UART interface for the specific device.

Arguments
~~~~~~~~~
    - Device path (``dev``)
    - Baud rate (``baud``)
    - Options string (``opt``)

The first character from the options strings are the number of data bits (current valid
number of data bits are ``5``, ``6``, ``7`` and ``8``), the second character is the parity
(valid characters are ``N`` (for none), ``O`` (for odd), ``E`` (for even)), the third
character is the number of stop bits (current valid number are ``1`` and ``2``) and the
last character represent the flow control (valid character are ``N`` (for none), ``S``
(for software), ``H`` (for hardware)).

Returns
~~~~~~~
Returns a valid UART object (handle), or ``NULL`` if an error occurred.

Usage
~~~~~

.. code-block:: c

    uart_t *uart_obj;

    uart_obj = UART_open("/dev/ttyS0", UART_BAUD_115200, "8N1N");

Function ``void UART_close(uart_t *uart)``
------------------------------------------

Description
~~~~~~~~~~~
Closes the UART interface and frees the UART object/handle.

Arguments
~~~~~~~~~
    - UART object/handle (``uart``)

Returns
~~~~~~~
None

Usage
~~~~~

.. code-block:: c

    UART_close(uart_obj);

Function ``ssize_t UART_send(uart_t *uart, char *send_buf, size_t len)``
------------------------------------------------------------------------

Description
~~~~~~~~~~~
Send data from ``send_buf`` over UART connection.

Arguments
~~~~~~~~~
    - UART object/handle (``uart``)
    - Send buffer (``send_buf``)
    - Length of buffer (``len``)

Returns
~~~~~~~
Returns number of sent bytes, or ``-1`` if an error occurred.

Usage
~~~~~

.. code-block:: c

    UART_send(uart_obj, buf, 256);

Function ``ssize_t UART_recv(uart_t *uart, char *recv_buf, size_t len)``
------------------------------------------------------------------------

Description
~~~~~~~~~~~
Receive over UART and store the data in ``recv_buf``.

Arguments
~~~~~~~~~
    - UART object/handle (``uart``)
    - Receive buffer (``recv_buf``)
    - Length of buffer (``len``)

Returns
~~~~~~~
Returns number of received bytes, or ``-1`` if an error occurred.

Usage
~~~~~

.. code-block:: c

    UART_recv(uart_obj, buf, 256);

Function ``ssize_t UART_puts(uart_t *uart, char *msg)``
-------------------------------------------------------

Description
~~~~~~~~~~~
Send a string over UART.

Arguments
~~~~~~~~~
    - UART object/handle (``uart``)
    - String to send (``msg``)

Returns
~~~~~~~
Returns number of sent bytes, or ``-1`` if an error occurred.

Usage
~~~~~

.. code-block:: c

    UART_puts(uart_obj, buf, 256);

Function ``int UART_putc(uart_t *uart, char c)``
-------------------------------------------------------

Description
~~~~~~~~~~~
Send a single character over UART.

Arguments
~~~~~~~~~
    - UART object/handle (``uart``)
    - Character to send (``c``)

Returns
~~~~~~~
Returns number of sent bytes, or ``-1`` if an error occurred.

Usage
~~~~~

.. code-block:: c

    UART_putc(uart_obj, 'A');

Function ``int UART_getc(uart_t *uart, char *ret_c)``
-----------------------------------------------------

Description
~~~~~~~~~~~
Get character from UART.

Arguments
~~~~~~~~~
    - UART object/handle (``uart``)
    - Pointer to character (``ret_t``)

Returns
~~~~~~~
Returns number of received bytes, or ``-1`` if an error occurred.

Usage
~~~~~

.. code-block:: c

    char c;

    UART_getc(uart_obj, &c);

Function ``int UART_flush(uart_t *uart)``
-----------------------------------------

Description
~~~~~~~~~~~
Flush not sent data over the UART.

Arguments
~~~~~~~~~
    - UART object/handle (``uart``)

Returns
~~~~~~~
Returns ``0`` on success, or ``-1`` if an error occurred.

Usage
~~~~~

.. code-block:: c

    UART_flush(uart_obj);

Function ``int UART_set_baud(uart_t *uart, enum e_baud baud)``
--------------------------------------------------------------

Description
~~~~~~~~~~~
Set the baud rate.

Arguments
~~~~~~~~~
    - UART object/handle (``uart``)
    - Baud rate (``baud``)

Returns
~~~~~~~
Returns ``0`` on success, or ``-1`` if an error occurred.

Usage
~~~~~

.. code-block:: c

    UART_set_baud(uart_obj, UART_BAUD_115200);

Function ``int UART_get_baud(uart_t *uart, int *ret_baud)``
-----------------------------------------------------------

Description
~~~~~~~~~~~
Returns the baud rate in ``ret_baud``.

Arguments
~~~~~~~~~
    - UART object/handle (``uart``)
    - Pointer to baud rate (``ret_baud``)

Returns
~~~~~~~
Returns ``0`` on success, or ``-1`` if an error occurred.

Usage
~~~~~

.. code-block:: c

    int baud;

    UART_get_baud(uart_obj, &baud);

Function ``int UART_get_fd(uart_t *uart, int *ret_fd)`` (Linux only)
--------------------------------------------------------------------

Description
~~~~~~~~~~~
Get the underlying file descriptor for the UART.

Arguments
~~~~~~~~~
    - UART object/handle (``uart``)
    - Pointer to the file descriptor (``ret_fd``)

Returns
~~~~~~~
Returns ``0`` on success, or ``-1`` if an error occurred.

Usage
~~~~~

.. code-block:: c

    int fd;

    UART_get_fd(uart_obj, &fd);

Function ``int UART_get_handle(uart_t *uart, HANDLE *ret_h)`` (Windows only)
--------------------------------------------------------------------------

Description
~~~~~~~~~~~
Get the underlying file handle for the UART.

Arguments
~~~~~~~~~
    - UART object/handle (``uart``)
    - Pointer to the file handle (``ret_h``)

Returns
~~~~~~~
Returns ``0`` on success, or ``-1`` if an error occurred.

Usage
~~~~~

.. code-block:: c

    HANDLE fd;

    UART_get_fd(uart_obj, &ret_h);

Function ``int UART_get_dev(uart_t *uart, char **ret_dev)``
-----------------------------------------------------------

Description
~~~~~~~~~~~
Get the UART device name in ``ret_dev``.

Arguments
~~~~~~~~~
    - UART object/handle (``uart``)
    - Pointer to the device name (``ret_dev``)

Returns
~~~~~~~
Returns ``0`` on success, or ``-1`` if an error occurred.

Usage
~~~~~

.. code-block:: c

    int *dev;

    UART_get_dev(uart_obj, &dev);

Function ``int UART_set_databits(uart_t *uart, enum e_data data_bits)``
-----------------------------------------------------------------------

Description
~~~~~~~~~~~
Set the UART data bits.

Arguments
~~~~~~~~~
    - UART object/handle (``uart``)
    - UART number of data bits (``data_bits``)
        - ``UART_DATA_5``
        - ``UART_DATA_6``
        - ``UART_DATA_7``
        - ``UART_DATA_8``
        - ``UART_DATA_16``

Returns
~~~~~~~
Returns ``0`` on success, or ``-1`` if an error occurred.

Usage
~~~~~

.. code-block:: c

    UART_set_databits(uart_obj, UART_BAUD_115200);

Function ``int UART_get_databits(uart_t *uart, int *ret_data_bits)``
--------------------------------------------------------------------

Description
~~~~~~~~~~~
Returns the data bits of the UART in ``ret_data_bits``.

Arguments
~~~~~~~~~
    - UART object/handle (``uart``)
    - Pointer to number of data bits (``ret_data_bits``)

Returns
~~~~~~~
Returns ``0`` on success, or ``-1`` if an error occurred.

Usage
~~~~~

.. code-block:: c

    int data;

    UART_get_databits(uart_obj, &data);

Function ``int UART_set_parity(uart_t *uart, enum e_parity parity)``
--------------------------------------------------------------------

Description
~~~~~~~~~~~
Set the UART parity.

Arguments
~~~~~~~~~
    - UART object/handle (``uart``)
    - UART parity (``parity``)
        - ``UART_PARITY_NONE``
        - ``UART_PARITY_ODD``
        - ``UART_PARITY_EVEN``

Returns
~~~~~~~
Returns ``0`` on success, or ``-1`` if an error occurred.

Usage
~~~~~

.. code-block:: c

    UART_set_parity(uart_obj, UART_PARITY_NONE);

Function ``int UART_get_parity(uart_t *uart, int *ret_parity)``
---------------------------------------------------------------

Description
~~~~~~~~~~~

Returns the UART parity in ``ret_parity``.

Arguments
~~~~~~~~~
    - UART object/handle (``uart``)
    - Pointer to parity (``ret_parity``)

Returns
~~~~~~~
Returns ``0`` on success, or ``-1`` if an error occurred.

Usage
~~~~~

.. code-block:: c

    int parity;

    UART_get_parity(uart_obj, &parity);

Function ``int UART_set_stopbits(uart_t *uart, enum e_stop stop_bits)``
-----------------------------------------------------------------------

Description
~~~~~~~~~~~
Set the number of stop bits.

Arguments
~~~~~~~~~
    - UART object/handle (``uart``)
    - UART number of stop bits (``stop_bits``)
        - ``UART_STOP_1_0``
        - ``UART_STOP_1_5``
        - ``UART_STOP_2_0``

Returns
~~~~~~~
Returns ``0`` on success, or ``-1`` if an error occurred.

Usage
~~~~~

.. code-block:: c

    UART_set_stopbits(uart_obj, UART_STOP_1_0);

Function ``int UART_get_stopbits(uart_t *uart, int *ret_stop_bits)``
--------------------------------------------------------------------

Description
~~~~~~~~~~~
Get the number of stop bits in ``ret_stop_bits``.

Arguments
~~~~~~~~~
    - UART object/handle (``uart``)
    - Pointer to stop bits (``ret_stop_bits``)

Returns
~~~~~~~
Returns ``0`` on success, or ``-1`` if an error occurred.

Usage
~~~~~

.. code-block:: c

    int stop;

    UART_get_stopbits(uart_obj, &stop);

Function ``int UART_set_flowctrl(uart_t *uart, enum e_flow flow_ctrl)``
-----------------------------------------------------------------------

Description
~~~~~~~~~~~
Set the flow control.

Arguments
~~~~~~~~~
    - UART object/handle (``uart``)
    - UART flow control (``flow_ctrl``)
        - ``UART_FLOW_NO`` (none)
        - ``UART_FLOW_SW`` (software)
        - ``UART_FLOW_HW`` (hardware)

Returns
~~~~~~~
Returns ``0`` on success, or ``-1`` if an error occurred.

Usage
~~~~~

.. code-block:: c

    UART_set_flowctrl(uart_obj, UART_FLOW_NO);

Function ``int UART_get_flowctrl(uart_t *uart, int *ret_flow_ctrl)``
--------------------------------------------------------------------

Description
~~~~~~~~~~~
Returns the flow control in ``ret_flow_ctrl``.

Arguments
~~~~~~~~~
    - UART object/handle (``uart``)
    - Pointer to flow control (``ret_flow_ctrl``)

Returns
~~~~~~~
Returns ``0`` on success, or ``-1`` if an error occurred.

Usage
~~~~~

.. code-block:: c

    int flow;

    UART_get_flowctrl(uart_obj, &flow);


Function ``int UART_set_pin(uart_t *uart, enum e_pins pin, int state)``
-----------------------------------------------------------------------

Description
~~~~~~~~~~~
Set the UART pin state.

Arguments
~~~~~~~~~
    - UART object/handle (``uart``)
    - UART pin (``pin``)
        - ``UART_PIN_RTS`` (out)
        - ``UART_PIN_DTR`` (out)
    - Pin state (``state``)
        - ``UART_PIN_LOW``
        - ``UART_PIN_HIGH``

Returns
~~~~~~~
Returns ``0`` on success, or ``-1`` if an error occurred.

Usage
~~~~~

.. code-block:: c

    int state;

    UART_set_pin(uart_obj, UART_PIN_RTS, UART_PIN_HIGH);


Function ``int UART_get_pin(uart_t *uart, enum e_pins pin, int *ret_state)``
----------------------------------------------------------------------------

Description
~~~~~~~~~~~
Get the UART pin state.

Arguments
~~~~~~~~~
    - UART object/handle (``uart``)
    - UART pin (``pin``)
        - ``UART_PIN_RTS``
        - ``UART_PIN_CTS``
        - ``UART_PIN_DSR``
        - ``UART_PIN_DCD``
        - ``UART_PIN_DTR``
        - ``UART_PIN_RI``
    - Pointer to pin state (``ret_state``). Possible values are:
        - ``UART_PIN_LOW``
        - ``UART_PIN_HIGH``

Returns
~~~~~~~
Returns ``0`` on success, or ``-1`` if an error occurred.

Usage
~~~~~

.. code-block:: c

    int state;

    UART_get_pin(uart_obj, UART_PIN_CTS, &state);

Function ``int UART_get_bytes_available(uart_t *uart, int *ret_num)``
---------------------------------------------------------------------

Description
~~~~~~~~~~~
Returns the number in ``ret_num`` of bytes available.

Arguments
~~~~~~~~~
    - UART object/handle (``uart``)
    - Pointer to received bytes (``ret_num``)

Returns
~~~~~~~
Returns ``0`` on success, or ``-1`` if an error occurred.

Usage
~~~~~

.. code-block:: c

    int bytes;

    UART_get_bytes_available(uart_obj, &bytes);

Function ``void UART_set_errmsg(int msg_enable)``
-------------------------------------------------

Description
~~~~~~~~~~~
This is a stub, currently not used.

Returns
~~~~~~~
None.

Function ``char *UART_get_libname(void)``
-----------------------------------------

Description
~~~~~~~~~~~
Returns the library name.

Arguments
~~~~~~~~~
    - None

Returns
~~~~~~~
Returns the library name string.

Usage
~~~~~

.. code-block:: c

    printf("%s", UART_get_libname());

Function ``char *UART_get_libversion(void)``
--------------------------------------------

Description
~~~~~~~~~~~
Returns the library version.

Arguments
~~~~~~~~~
    - None

Returns
~~~~~~~
Returns the library version string.

Usage
~~~~~

.. code-block:: c

    printf("%s", UART_get_libversion());
