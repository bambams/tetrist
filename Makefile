ALLEGRO_LIBS = allegro-5 allegro_primitives-5
CC = gcc
CFLAGS = $$(pkg-config --cflags $(ALLEGRO_LIBS))

ifdef DEBUG
	CFLAGS += -g
endif

EXE = game
LIBS = $$(pkg-config --libs $(ALLEGRO_LIBS))
OBJECTS = $(shell find -name '*.c' | sed -e 's/\.c$$/.o/')

all: $(EXE)

clean:
	rm -fR $(EXE) $(OBJECTS)

$(EXE): $(OBJECTS)
	$(CC) -o $@ $? $(LIBS)

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<
