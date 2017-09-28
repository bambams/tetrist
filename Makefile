ALLEGRO_LIBS = allegro-5 allegro_dialog-5 allegro_primitives-5
CC = gcc
CFLAGS = $$(pkg-config --cflags $(ALLEGRO_LIBS)) -Wall

ifdef DEBUG
	CFLAGS += -g
endif

EXE = ./game
LIBS = $$(pkg-config --libs $(ALLEGRO_LIBS))
OBJECTS = $(shell find -name '*.c' | sed -e 's/\.c$$/.o/')

all: clear $(EXE)

clean:
	rm -fR $(EXE) $(OBJECTS)

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
	$(CC) -o $@ $? $(LIBS)

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<
