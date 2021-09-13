# SPDX-License-Identifier: GPL-2.0-or-later
CFLAGS = -O -Wall -Isrc -lpthread
example = example-one example-simple

all: FORCE

scoro.a: src/scoro.c
	gcc -g $(CFLAGS) -o scoro.o -c $<
	ar crv scoro.a scoro.o

$(example): scoro.a
	gcc -g $(CFLAGS) -o $@ example/$@.c scoro.a 

all: $(example)

clean: FORCE
	rm -f $(example) scoro.a scoro.o

PHONY += FORCE
FORCE:

.PHONY: $(PHONY)
