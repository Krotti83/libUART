# libUART

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

## Build status

| Target/Configuration      | Status |
|---------------------------|--------|
| Linux/Default             | [![Build Ubuntu CI](https://github.com/Krotti83/libUART/actions/workflows/build_lnx_default.yml/badge.svg)](https://github.com/Krotti83/libUART/actions/workflows/build_lnx_default.yml) |
| Linux/Threading Support   | [![Build Ubuntu with Threads CI](https://github.com/Krotti83/libUART/actions/workflows/build_lnx_threads.yml/badge.svg)](https://github.com/Krotti83/libUART/actions/workflows/build_lnx_threads.yml) |
| Windows/Default           | [![Build for Windows on Ubuntu CI](https://github.com/Krotti83/libUART/actions/workflows/build_win_default.yml/badge.svg)](https://github.com/Krotti83/libUART/actions/workflows/build_win_default.yml) |
| Windows/Threading Support | [![Build for Windows on Ubuntu with Threads CI](https://github.com/Krotti83/libUART/actions/workflows/build_win_threads.yml/badge.svg)](https://github.com/Krotti83/libUART/actions/workflows/build_win_threads.yml) |

## NOTES

The library is still under development and not fully tested. But most of the functions
which are provided from the library should be working. If you investigate issue with the
functions from the library please report them to the [GitHub issue tracker](https://github.com/Krotti83/libUART/issues) for this repository.

## Supported operating systems

* **Linux**
* **FreeBSD**
* **Windows** (cross building through **MINGW** currently)

## Features

* Supports up to *512* UART's
* Threading support (experimental)

## TODO

* Add **Visual Studio** support (currently only cross building through **MINGW** is supported)
* Fully complete documentation (is for API version ``0.1.x.x``, and therefore not valid)

## Issues

If you find issues in the library please report them to the [GitHub issue tracker](https://github.com/Krotti83/libUART/issues) for this repository.


## Prerequisites for building the library (Linux)

* Git
* GNU make
* GNU binutils or LLVM binutils
* GCC or LLVM C compiler (clang)
* GCC-MINGW for cross building for Windows on Linux (target ``x86_64-w64-mingw32-``)
* Wine (only required if building for Windows)
* ``pdflatex`` (only required if the PDF documentation should be build)

### Ubuntu

```
$ sudo apt update
$ sudo apt install git wine binutils-mingw-w64-x86-64 gcc-mingw-w64-x86-64
```

The packages ``wine``, ``binutils-mingw-w64-x86-64`` and ``gcc-mingw-w64-x86-64`` are required
if building for **Windows** on Linux.

## Prerequisites for building the library (FreeBSD)

* Git
* GNU make (FreeBSD's make is currently not supported)
* LLVM binutils
* LLVM C compiler (clang)
* ``pdflatex`` (only required if the PDF documentation should be build)

```
$ pkg update
$ pkg install git
$ pkg install gmake
$ pkg install llvm
```

## Building the library (Linux)

### Build without documentation

With the *default* configuration, the PDF documentation will not be build. But the API documentation is
still available under [doc/libUART.rst](https://github.com/Krotti83/libUART/blob/main/doc/libUART.rst). 
Only the **LaTeX** PDF document will not be build.

```
$ ./configure
$ make
```
#### Output Files

The created files can be found in the ``build`` directory. The following files will be created:

* ``UART.h``
* ``libUART.so`` (Symbolic link to ``libUART.so.0``)
* ``libUART.so.0`` (Symbolic link to ``libUART.so.0.2``)
* ``libUART.so.0.2``
* ``libUART.a``

### Build with documentation

To build the library with PDF documentation generation, pass the option ``--enable-doc`` to the
``configure`` script. The ``configure`` also checks if the required application ``pdflatex`` is
installed for building the PDF documentation.


```
$ ./configure --enable-doc
$ make
```

#### Output Files

The created files can be found in the ``build`` directory. The following files will be created:

* ``UART.h``
* ``libUART.so`` (Symbolic link to ``libUART.so.0``)
* ``libUART.so.0`` (Symbolic link to ``libUART.so.0.2``)
* ``libUART.so.0.2``
* ``libUART.a``
* ``libUART.pdf``

### Build for Windows on Linux

For cross building on **Linux** for **Windows** pass the option ``--target=windows`` to
the ``configure`` script.

```
$ ./configure --target=windows
$ make
```

#### Output Files

The created files can be found in the ``build`` directory. The following files will be created:

* ``UART.h``
* ``libUART.dll``
* ``libUART.lib``
* ``libUART.pdf`` if building with option ``--enable-doc``

## Building the library (FreeBSD)

### Build without documentation

With the *default* configuration, the PDF documentation will not be build. But the API documentation is
still available under [doc/libUART.rst](https://github.com/Krotti83/libUART/blob/main/doc/libUART.rst). 
Only the **LaTeX** PDF document will not be build.

```
$ ./configure
$ gmake
```
#### Output Files

The created files can be found in the ``build`` directory. The following files will be created:

* ``UART.h``
* ``libUART.so`` (Symbolic link to ``libUART.so.0``)
* ``libUART.so.0`` (Symbolic link to ``libUART.so.0.2``)
* ``libUART.so.0.2``
* ``libUART.a``

### Build with documentation

To build the library with PDF documentation generation, pass the option ``--enable-doc`` to the
``configure`` script. The ``configure`` also checks if the required application ``pdflatex`` is
installed for building the PDF documentation.


```
$ ./configure --enable-doc
$ gmake
```

#### Output Files

The created files can be found in the ``build`` directory. The following files will be created:

* ``UART.h``
* ``libUART.so`` (Symbolic link to ``libUART.so.0``)
* ``libUART.so.0`` (Symbolic link to ``libUART.so.0.2``)
* ``libUART.so.0.2``
* ``libUART.a``
* ``libUART.pdf``

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
    int ret;
    uart_t *uart;
    
    /* Initialize the library */
    ret = UART_init();
    
    if (ret != UART_ESUCCESS) {
        return 1;
    }
    
    /* Open the UART port */
    uart = UART_open("/dev/ttyS0", UART_BAUD_115200, "8N1N");
    
    if (!uart) {
        return 1;
    }
    
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

### Link against the library (dynamic/shared)

Example if you want to link dynamically to the library if the library isn't installed
on your system.

```
$ gcc -I build -o test test.c build/libUART.so
```

## Documentation

The documentation is currently not completed, but will finalize this later too. The current
API (Application Programming Interface) can be found [here (doc/libUART.rst)](https://github.com/Krotti83/libUART/blob/main/doc/libUART.rst).
