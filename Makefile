# SPDX-License-Identifier: GPL-2.0-or-later
CFLAGS = -O -Wall

all: example-simple
example-simple: scoro.c example-simple.c -lpthread

clean:
	rm -f example-simple
