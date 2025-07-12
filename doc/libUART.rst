===========
libUART API
===========

``void UART_init(void)``
------------------------
Initializes the UART library.

``uart_t *UART_open(const char *dev, int baud, const char *opt)``
-----------------------------------------------------------------
Opens the UART interface.

``void UART_close(uart_t *uart)``
---------------------------------
Closes the UART interface.

``int UART_send(uart_t *uart, char *send_buf, int len)``
--------------------------------------------------------
Send data from ``send_buf`` over UART connection.

``int UART_recv(uart_t *uart, char *recv_buf, int len)``
--------------------------------------------------------
Receive over UART and store the data in ``recv_buf``.

``int UART_puts(uart_t *uart, char *msg)``
------------------------------------------
Put string over UART.

``int UART_getc(uart_t *uart, char *ret_c)``
--------------------------------------------
Get char from UART.

``int UART_flush(uart_t *uart)``
--------------------------------
Flush not sent data.

``int UART_set_baud(uart_t *uart, int baud)``
---------------------------------------------
Set the baud rate.

``int UART_get_baud(uart_t *uart, int *ret_baud)``
--------------------------------------------------
Returns the baud rate in ``ret_baud``.

``int UART_get_fd(uart_t *uart, int *ret_fd)``
----------------------------------------------
Get the underlying file descriptor for the UART.

``int UART_get_dev(uart_t *uart, char **ret_dev)``
--------------------------------------------------
Get the UART device name in ``ret_dev``.

``int UART_set_databits(uart_t *uart, int data_bits)``
------------------------------------------------------
Set the UART data bits.

``int UART_get_databits(uart_t *uart, int *ret_data_bits)``
-----------------------------------------------------------
Returns the data bits of the UART in ``ret_data_bits``.

``int UART_set_parity(uart_t *uart, int parity)``
-------------------------------------------------
Set the parity.

``int UART_get_parity(uart_t *uart, int *ret_parity)``
------------------------------------------------------
Returns the parity in ``ret_parity``.

``int UART_set_stopbits(uart_t *uart, int stop_bits)``
------------------------------------------------------
Set the number of stop bits.

``int UART_get_stopbits(uart_t *uart, int *ret_stop_bits)``
-----------------------------------------------------------
Get the number of stop bits in ``ret_stop_bits``.

``int UART_set_flowctrl(uart_t *uart, int flow_ctrl)``
------------------------------------------------------
Set the flow control.

``int UART_get_flowctrl(uart_t *uart, int *ret_flow_ctrl)``
-----------------------------------------------------------
Returns the flow control in ``ret_flow_ctrl``.

``int UART_set_pin(uart_t *uart, int pin, int state)``
------------------------------------------------------
Set the UART pin state.

``int UART_get_pin(uart_t *uart, int pin, int *ret_state)``
-----------------------------------------------------------
Get the UART pin state.

``int UART_get_bytes_available(uart_t *uart, int *ret_num)``
------------------------------------------------------------
Returns the number in ``ret_num`` of bytes available.

``void UART_set_errmsg(int msg_enable)``
----------------------------------------
This is a stub, currently not used.

``char *UART_get_libname(void)``
--------------------------------
Returns the library name.

``char *UART_get_libversion(void)``
-----------------------------------
Returns the library version.
