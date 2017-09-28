#ifndef TILE_MAP_H
    #define TILE_MAP_H

#define TILE_MAP_ADDRESS(w, x, y) (y * w + x)

typedef struct {
    int w, h;
    char * map;
} TILE_MAP;

int tile_map_create(TILE_MAP **, int, int, const char * const);
void tile_map_destroy(TILE_MAP **);
char tile_map_get(TILE_MAP *, int, int);
char tile_map_get_aux(const char * const, int, int, int);
char tile_map_set(TILE_MAP *, int, int, char);
char tile_map_set_aux(char * const, int, int, int, char);

#endif
