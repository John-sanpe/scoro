# SPDX-License-Identifier: GPL-2.0-or-later
CFLAGS = -O -Wall -lpthread
HEADERS = list.h scoro.h scoro_osdep.h

all: example-simple
example-simple: scoro.c example-simple.c $(HEADERS)

clean:
	rm -f example-simple
