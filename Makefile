ALLEGRO_LIBS = allegro-5 allegro_dialog-5 allegro_primitives-5
CC = gcc
CFLAGS = $$(pkg-config --cflags $(ALLEGRO_LIBS))

ifdef DEBUG
	CFLAGS += -g
endif

EXE = ./game
LIBS = $$(pkg-config --libs $(ALLEGRO_LIBS))
OBJECTS = $(shell find -name '*.c' | sed -e 's/\.c$$/.o/')

all: $(EXE)

clean:
	rm -fR $(EXE) $(OBJECTS)

debug:
	DEBUG=1 $(MAKE) clean all && gdb $(EXE)

run: all
	$(EXE)

rerun: clean all $(EXE)
	$(EXE)

$(EXE): $(OBJECTS)
	$(CC) -o $@ $? $(LIBS)

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<
