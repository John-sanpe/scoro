# SPDX-License-Identifier: GPL-2.0-or-later
CFLAGS = -O -Wall -Isrc


all: scoro example

scoro:
	gcc -c -g -I$(CFLAGS) src/scoro.c -lpthread -o scoro.o
	ar crv scoro.a scoro.o
	rm -rf scoro.o 
example:
	gcc -g -I$(CFLAGS) example/example-simple.c scoro.a -o example-simple -lpthread



clean:
	rm -f example-simple
.PHONY: example