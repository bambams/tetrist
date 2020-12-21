ifeq ($(OS),Windows_NT)
	ALLEGRO_LIBS = -LD:/Users/Brandon/bin -lallegro_monolith-5.2
else
	ALLEGRO_LIBS = allegro-5 allegro_dialog-5 allegro_font-5 allegro_main-5 allegro_primitives-5 allegro_ttf-5
endif

BUILDDIR = .build
CC = gcc
CFLAGS = -Wall
EXE = ./game

ifeq ($(OS),Windows_NT)
	EXE := $(EXE).exe
else
	CFLAGS += $$(pkg-config --cflags $(ALLEGRO_LIBS))
endif

ifdef DEBUG
	CFLAGS += -DDEBUG -g
endif

LIBS = -lm

ifeq ($(OS),Windows_NT)
	LIBS += $(ALLEGRO_LIBS)
else
	LIBS += $$(pkg-config --libs $(ALLEGRO_LIBS))
endif


OBJECTS = $(shell find -maxdepth 1 -name '*.c' | sed -e 's/^/$(BUILDDIR)\//' -e 's/\.c$$/.o/')
SHELL = ./build-shell.bash

all: clear $(EXE)

clean:
	rm -fR $(BUILDDIR) $(EXE) $(OBJECTS)

clear:
	if [ ! -z "$$MAKE_ALL_CLEAR" ]; then clear; fi;

debug:
	DEBUG=1 $(MAKE) clean all && gdb $(EXE)

memcheck: clean all
	valgrind --leak-check=full --track-origins=yes $(EXE)

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
