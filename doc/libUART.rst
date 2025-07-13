===============================================
libUART API (Application Programming Interface)
===============================================

libUART header
--------------

Include libUART as following to use the library:

``#include <UART.h>`` or ``#include "UART.h"``

Function ``void UART_init(void)``
---------------------------------

Description
~~~~~~~~~~~

Initializes the UART library.

Usage
~~~~~::
    UART_init();

Arguments
~~~~~~~~~
    - None

Notes
~~~~~
This function is currently a stub. It isn't really required, but should
be used for further API changes.

Function ``uart_t *UART_open(const char *dev, enum e_baud baud, const char *opt)``
--------------------------------------------------------------------------

Description
~~~~~~~~~~~

Opens the UART interface for the specific device.

Arguments
~~~~~~~~~
    - Device (``dev``)
    - Baud rate (``baud``)
    - Options string (``opt``)

The first character from the options strings are the number of data bits (current valid
number of data bits are ``5``, ``6``, ``7`` and ``8``), the second character is the parity (valid characters
are ``N`` (for none), ``O`` (for odd), ``E`` (for even)), the third character is the number of stop
bits (current valid number are ``1`` and ``2``) and the last character represent the flow control
(valid character are ``N`` (for none), ``S`` (for software), ``H`` (for hardware)).

Usage
~~~~~::

    uart_t *uart_obj;

    uart_obj = UART_init("/dev/ttyS0", UART_BAUD_115200, "8N1N");

Function ``void UART_close(uart_t *uart)``
------------------------------------------

Description
~~~~~~~~~~~

Closes the UART interface.

Function ``ssize_t UART_send(uart_t *uart, char *send_buf, size_t len)``
------------------------------------------------------------------------

Description
~~~~~~~~~~~

Send data from ``send_buf`` over UART connection.

Function ``ssize_t UART_recv(uart_t *uart, char *recv_buf, size_t len)``
------------------------------------------------------------------------

Description
~~~~~~~~~~~

Receive over UART and store the data in ``recv_buf``.

Function ``ssize_t UART_puts(uart_t *uart, char *msg)``
-------------------------------------------------------

Description
~~~~~~~~~~~

Put string over UART.

Function ``int UART_getc(uart_t *uart, char *ret_c)``
-----------------------------------------------------

Description
~~~~~~~~~~~

Get char from UART.

Function ``int UART_flush(uart_t *uart)``
-----------------------------------------

Description
~~~~~~~~~~~

Flush not sent data.

Function ``int UART_set_baud(uart_t *uart, enum e_baud baud)``
--------------------------------------------------------------

Description
~~~~~~~~~~~

Set the baud rate.

Function ``int UART_get_baud(uart_t *uart, int *ret_baud)``
-----------------------------------------------------------

Description
~~~~~~~~~~~

Returns the baud rate in ``ret_baud``.

Function ``int UART_get_fd(uart_t *uart, int *ret_fd)``
-------------------------------------------------------

Description
~~~~~~~~~~~

Get the underlying file descriptor for the UART.

Function ``int UART_get_dev(uart_t *uart, char **ret_dev)``
-----------------------------------------------------------

Description
~~~~~~~~~~~

Get the UART device name in ``ret_dev``.

Function ``int UART_set_databits(uart_t *uart, enum e_data data_bits)``
-----------------------------------------------------------------------

Description
~~~~~~~~~~~

Set the UART data bits.

Function ``int UART_get_databits(uart_t *uart, int *ret_data_bits)``
--------------------------------------------------------------------

Description
~~~~~~~~~~~

Returns the data bits of the UART in ``ret_data_bits``.

Function ``int UART_set_parity(uart_t *uart, enum e_parity parity)``
--------------------------------------------------------------------

Description
~~~~~~~~~~~

Set the parity.

Function ``int UART_get_parity(uart_t *uart, int *ret_parity)``
---------------------------------------------------------------

Description
~~~~~~~~~~~

Returns the parity in ``ret_parity``.

Function ``int UART_set_stopbits(uart_t *uart, enum e_stop stop_bits)``
-----------------------------------------------------------------------

Description
~~~~~~~~~~~

Set the number of stop bits.

Function ``int UART_get_stopbits(uart_t *uart, int *ret_stop_bits)``
--------------------------------------------------------------------

Description
~~~~~~~~~~~

Get the number of stop bits in ``ret_stop_bits``.

Function ``int UART_set_flowctrl(uart_t *uart, enum e_flow flow_ctrl)``
-----------------------------------------------------------------------

Description
~~~~~~~~~~~

Set the flow control.

Function ``int UART_get_flowctrl(uart_t *uart, int *ret_flow_ctrl)``
--------------------------------------------------------------------

Description
~~~~~~~~~~~

Returns the flow control in ``ret_flow_ctrl``.

Function ``int UART_set_pin(uart_t *uart, enum e_pins pin, int state)``
-----------------------------------------------------------------------

Description
~~~~~~~~~~~

Set the UART pin state.

Function ``int UART_get_pin(uart_t *uart, enum e_pins pin, int *ret_state)``
----------------------------------------------------------------------------

Description
~~~~~~~~~~~

Get the UART pin state.

Function ``int UART_get_bytes_available(uart_t *uart, int *ret_num)``
---------------------------------------------------------------------

Description
~~~~~~~~~~~

Returns the number in ``ret_num`` of bytes available.

Function ``void UART_set_errmsg(int msg_enable)``
-------------------------------------------------

Description
~~~~~~~~~~~

This is a stub, currently not used.

Function ``char *UART_get_libname(void)``
-----------------------------------------

Description
~~~~~~~~~~~

Returns the library name.

Function ``char *UART_get_libversion(void)``
--------------------------------------------

Description
~~~~~~~~~~~

Returns the library version.
