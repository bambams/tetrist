ALLEGRO_LIBS = allegro-5 allegro_dialog-5 allegro_primitives-5
BUILDDIR = .build
CC = gcc
CFLAGS = $$(pkg-config --cflags $(ALLEGRO_LIBS)) -I.. -Wall

ifdef DEBUG
	CFLAGS += -g
endif

LIBS = $$(pkg-config --libs $(ALLEGRO_LIBS)) ../$(BUILDDIR)/list.o
OBJECTS = $(shell find -maxdepth 1 -name '*.c' | sed -e 's/^/$(BUILDDIR)\//' -e 's/\.c$$/.o/')
TESTS = $(shell find -maxdepth 1 -name '*.c' | sed -e 's/\.c$$/.test/')
SHELL = ../build-shell.bash

all: clear $(TESTS)

clean:
	rm -fR $(BUILDDIR) $(OBJECTS) $(TESTS)

clear:
	if [ ! -z "$$MAKE_ALL_CLEAR" ]; then clear; fi;

debug:
	DEBUG=1 $(MAKE) clean all && for f in $(TESTS); do echo "$$f:" && gdb "./$$f" || break; done

memcheck: clean all
	for f in $(TESTS); do echo "$$f:" && valgrind --leak-check=full --track-origins=yes "./$$f" || break; done

rebuild: clean all

run: all
	for f in $(TESTS); do echo "$$f:" && "./$$f" || break; done

rerun: rebuild run

%.test: $(BUILDDIR)/%.o
	$(CC) $(CFLAGS) -o $@ $? $(LIBS)

%.o: %.c

$(BUILDDIR)/%.c: %.c
	$(CC) -E $(CFLAGS) -o $@ $<

$(BUILDDIR)/%.o: $(BUILDDIR)/%.c
	$(CC) -c $(CFLAGS) -o $@ $<
