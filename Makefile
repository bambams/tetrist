ALLEGRO_LIBS = allegro-5 allegro_dialog-5 allegro_primitives-5
BUILDDIR = .build
CC = gcc
CFLAGS = $$(pkg-config --cflags $(ALLEGRO_LIBS)) -Wall

ifdef DEBUG
	CFLAGS += -g
endif

EXE = ./game
LIBS = $$(pkg-config --libs $(ALLEGRO_LIBS))
OBJECTS = $(shell find -maxdepth 1 -name '*.c' | sed -e 's/^/$(BUILDDIR)\//' -e 's/\.c$$/.o/')
SHELL = ./build-shell.bash

all: clear $(EXE)

clean:
	rm -fR $(BUILDDIR) $(EXE) $(OBJECTS)

clear:
	if [ ! -z "$$MAKE_ALL_CLEAR" ]; then clear; fi;

debug:
	DEBUG=1 $(MAKE) clean all && gdb $(EXE)

rebuild: clean all

run: all
	$(EXE)

rerun: rebuild
	$(EXE)

$(EXE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $? $(LIBS)

%.o: %.c

$(BUILDDIR)/%.c: %.c
	$(CC) -E $(CFLAGS) -o $@ $<

$(BUILDDIR)/%.o: $(BUILDDIR)/%.c
	$(CC) -c $(CFLAGS) -o $@ $<
