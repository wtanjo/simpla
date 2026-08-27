# simpla

A simple linear alagebra library written in C. Basically for learning and my personal use.

The major goal and principle is that it should provide simple interfaces for linear algebra computation.

## Usage

## Prerequisites
- gcc
- scdoc (for building manpage, see below)

Clone this repo and do `make` to compile the libraries. Do `(sudo) make install` if you want easy access to this library. You can modified [Makefile](./Makefile) according to your specific needs.

You can browse [simpla.h](./simpla.h) directly and guess out what the functions and the parameters does, and I believe you'll guess them right easily. Still, here's a manpage for you, but you'll need scdoc to build it. This is not set as default, and you'll need to do `make man` and `make man-install` to build and install it. After it you can do `man simpla` in any shell to check out the manual.

Yet another way to the manual is the builtin man function. Try in C code
```C
man("man");
```
to learn how to use this function.

## Performance

Although this library's major goal is not replacing any of the existing ones (OpenBlas, Intel MKL ...), the performance of a computational library is always of concern.

Here's some (informal) statistics on my personal computer (Dell Inspiron 5490, CPU: Intel Core i5-10210U, GPU: Intel UHD Graphics, 8GB memory, Fedora 44):

(NOT FINISHED...)
