ALLEGRO_LIBS = allegro-5 allegro_primitives-5
CC = gcc
CFLAGS = $$(pkg-config --cflags $(ALLEGRO_LIBS))
EXE = game
LIBS = $$(pkg-config --libs allegro-5 allegro_primitives-5)
OBJECTS = $(shell find -name '*.c' | sed -e 's/\.c$$/.o/')

all: $(EXE)

clean:
	rm -fR $(EXE) $(OBJECTS)

$(EXE): $(OBJECTS)
	$(CC) -o $@ $? $(LIBS)

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<
