===============================================
libUART API (Application Programming Interface)
===============================================

These document describes the application programming interface (API) for the ``libUART`` library.

Header ``libUART``
------------------

To use the library include the following header in your source code.

::
    #include <UART.h>

Function ``void UART_init(void)``
---------------------------------

Description
~~~~~~~~~~~

Initializes the UART library.

Arguments
~~~~~~~~~

    - None

Returns
~~~~~~~

None

Usage
~~~~~

::
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

::
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

::
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

::
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

::
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

::
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

::
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

::
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

::
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

::
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

::
    int baud;

    UART_get_baud(uart_obj, &baud);

Function ``int UART_get_fd(uart_t *uart, int *ret_fd)``
-------------------------------------------------------

Description
~~~~~~~~~~~

Get the underlying file descriptor for the UART.

Arguments
~~~~~~~~~

    - UART object/handle (``uart``)

Returns
~~~~~~~

Returns ``0`` on success, or ``-1`` if an error occurred.

Function ``int UART_get_dev(uart_t *uart, char **ret_dev)``
-----------------------------------------------------------

Description
~~~~~~~~~~~

Get the UART device name in ``ret_dev``.

Arguments
~~~~~~~~~

    - UART object/handle (``uart``)

Returns
~~~~~~~

Returns ``0`` on success, or ``-1`` if an error occurred.

Function ``int UART_set_databits(uart_t *uart, enum e_data data_bits)``
-----------------------------------------------------------------------

Description
~~~~~~~~~~~

Set the UART data bits.

Arguments
~~~~~~~~~

    - UART object/handle (``uart``)

Returns
~~~~~~~

Returns ``0`` on success, or ``-1`` if an error occurred.

Function ``int UART_get_databits(uart_t *uart, int *ret_data_bits)``
--------------------------------------------------------------------

Description
~~~~~~~~~~~

Returns the data bits of the UART in ``ret_data_bits``.

Arguments
~~~~~~~~~

    - UART object/handle (``uart``)

Returns
~~~~~~~

Returns ``0`` on success, or ``-1`` if an error occurred.


Function ``int UART_set_parity(uart_t *uart, enum e_parity parity)``
--------------------------------------------------------------------

Description
~~~~~~~~~~~

Set the parity.

Arguments
~~~~~~~~~

    - UART object/handle (``uart``)

Returns
~~~~~~~

Returns ``0`` on success, or ``-1`` if an error occurred.


Function ``int UART_get_parity(uart_t *uart, int *ret_parity)``
---------------------------------------------------------------

Description
~~~~~~~~~~~

Returns the parity in ``ret_parity``.

Arguments
~~~~~~~~~

    - UART object/handle (``uart``)

Returns
~~~~~~~

Returns ``0`` on success, or ``-1`` if an error occurred.


Function ``int UART_set_stopbits(uart_t *uart, enum e_stop stop_bits)``
-----------------------------------------------------------------------

Description
~~~~~~~~~~~

Set the number of stop bits.

Arguments
~~~~~~~~~

    - UART object/handle (``uart``)

Returns
~~~~~~~

Returns ``0`` on success, or ``-1`` if an error occurred.



Function ``int UART_get_stopbits(uart_t *uart, int *ret_stop_bits)``
--------------------------------------------------------------------

Description
~~~~~~~~~~~

Get the number of stop bits in ``ret_stop_bits``.

Arguments
~~~~~~~~~

    - UART object/handle (``uart``)

Returns
~~~~~~~

Returns ``0`` on success, or ``-1`` if an error occurred.


Function ``int UART_set_flowctrl(uart_t *uart, enum e_flow flow_ctrl)``
-----------------------------------------------------------------------

Description
~~~~~~~~~~~

Set the flow control.

Arguments
~~~~~~~~~

    - UART object/handle (``uart``)

Returns
~~~~~~~

Returns ``0`` on success, or ``-1`` if an error occurred.


Function ``int UART_get_flowctrl(uart_t *uart, int *ret_flow_ctrl)``
--------------------------------------------------------------------

Description
~~~~~~~~~~~

Returns the flow control in ``ret_flow_ctrl``.

Arguments
~~~~~~~~~

    - UART object/handle (``uart``)

Returns
~~~~~~~

Returns ``0`` on success, or ``-1`` if an error occurred.




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

::
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

::
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

::
    int bytes;

    UART_get_bytes_available(uart_obj, &bytes);

Function ``void UART_set_errmsg(int msg_enable)``
-------------------------------------------------

Description
~~~~~~~~~~~

This is a stub, currently not used.

Returns
~~~~~~~

none.

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
