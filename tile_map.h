#ifndef TILE_MAP_H
    #define TILE_MAP_H

    #define TILE_MAP_ADDRESS(w, x, y) (y * w + x)

struct TILE_MAP;
typedef struct TILE_MAP TILE_MAP;

    #include "misc.h"

struct TILE_MAP {
    SIZE size;
    char * map;
};

int tile_map_create(TILE_MAP **, int, int, const char * const);
void tile_map_destroy(TILE_MAP **);
char tile_map_get(TILE_MAP *, int, int);
char tile_map_get_aux(const char * const, int, int, int);
char tile_map_set(TILE_MAP *, int, int, char);
char tile_map_set_aux(char * const, int, int, int, char);

#endif
