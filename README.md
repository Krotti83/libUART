# libUART

Easy to use library for accessing the UART (serial interface)

## NOTES

The library is currently only build statically. Dynamic (shared library) support will be added later.

## Supported Operating Systems
* Linux (or all POSIX compliants)

## TODO
* Add Windows support
* Add shared library support
* Complete documentation

## Building

```
$ ./configure
$ make
```

## Link against the library

```
$ gcc -o test test.c libUART.a
```

## API Documentation

[![API Documentation](https://github.com/Krotti83/libUART/blob/main/doc/libUART.rst)]

## Status
[![Build CI](https://github.com/Krotti83/libUART/actions/workflows/build.yml/badge.svg)](https://github.com/Krotti83/libUART/actions/workflows/build.yml)

