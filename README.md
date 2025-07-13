[![Build CI](https://github.com/Krotti83/libUART/actions/workflows/build.yml/badge.svg)](https://github.com/Krotti83/libUART/actions/workflows/build.yml)

# libUART

Easy to use library for accessing the **UART** (serial interface). The library use the **POSIX**
functions ``termios`` on **Linux**. It should also possible to use the library on ***BSD**
systems too, but currently not tested. In the ``UART_open()`` function the library setups
the **UART** in *raw* and *none-blocking* mode currently.

## NOTES

The library is still under development and not fully tested. But most of the functions
which are provided for the library are fully usable.

## Supported operating systems

* **Linux** (or all **POSIX** compatible systems)

## TODO

* Add support for clang compiler
* Add threading support
* Add Windows support
* Fully complete documentation

## Prerequisites for building the library

* GNU make
* GNU binutils
* GCC (currently only building with GCC is supported)
* pdflatex (only required if the PDF documentation should be build too)

## Building the library
### Build without documentation

With the *default* setup, the PDF documentation will not be build. But the API documentation is
still available in under ``doc/libUART.rst``. Only the **LaTeX** document will not be build.

```
$ ./configure
$ make
```
### Build with documentation

To build the library with PDF documentation generation, pass the option ``--enable-doc`` to the
``configure`` script. The ``configure`` also checks if the required application ``pdflatex`` is
installed for building the PDF documentation too.


```
$ ./configure --enable-doc
$ make
```

## Install the library on your system

The **default** install path is under ``/usr/local`` but can changed *before* building the library
with the ``configure`` script. Pass the option ``--prefix=/path/to/install`` to the script.

```
$ make install
```

### NOTES

Depending on the install path it is sometimes required to install the library as **root** user.

## Example

### Usage from the library

A *short* example how to use the library.

```c
#include <UART.h>

int main(int c, char *argv[])
{
    uart_t *uart;
    
    /* Initialize the library */
    UART_init();
    
    /* Open the UART port */
    UART_open("/dev/ttyS0", UART_BAUD_115200, "8N1N");
    
    /* Send something over the UART */
    UART_puts(uart, "Hello World!\r\n");
    
    /* Close the UART port */
    UART_close(uart);
    
    return 0;
}
```

### Link against the library (static)

Example if you want to link statically to the library, if the library isn't installed
on your system.

```
$ gcc -I build -o test test.c build/libUART.a
```
Example if you want to link statically to the library if the library isn't installed
on your system.

### Link against the library (dynamic/shared)
```
$ gcc -I build -o test test.c build/libUART.so
```

## Documentation

The documentation is currently not completed, but will finalize this later too. The current
API (Application Programming Interface) can be found [![here](https://github.com/Krotti83/libUART/blob/main/doc/libUART.rst)].
