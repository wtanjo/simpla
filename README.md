# simpla

A simple linear alagebra library written in C. Basically for learning and my personal use.

The major goal and principle is that it should provide simple interfaces to linear algebra computation.

## Usage

Clone this repo and do `make` to make the libraries. Do `(sudo) make install` if you want easy access to this library. You can modified [Makefile](./Makefile) according to your specific needs.

You can browse [simpla.h](./simpla.h) directly and guess out what the functions and the parameters does, and I believe you'll guess them right easily. Still, if you want a manpage, here's one for you. If you want to do `man simpla` and see the manpage, you'll have to do `(sudo) make install`.

## Performance

Although this library's major goal is replacing any of the existing ones (OpenBlas, Intel MKL ...), the performance of a computational library is always of concern.

Here's some (informal) statistics on my personal computer (Dell Inspiron 5490, CPU: Intel Core i5-10210U, GPU: Intel UHD Graphics, 8GB Memory, Fedora 43):
