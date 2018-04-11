#include <assert.h>
#include <malloc.h>
#include <string.h>

#include "tile_map.h"

int tile_map_create(TILE_MAP ** tiles, int w, int h,
                    const char * const map) {
    assert(tiles);
    assert(!*tiles);

    *tiles = malloc(sizeof(TILE_MAP));

    if(*tiles == NULL) {
        return 0;
    }

#ifdef DEBUG
    fprintf(stderr, "%p malloc :t tile-map\n", *tiles);
#endif

    memset(*tiles, 0, sizeof(TILE_MAP));

    (*tiles)->size.w = w;
    (*tiles)->size.h = h;

    char ** data = &(*tiles)->map;

    *data = malloc(sizeof(char) * w * h);

    if(*data == NULL) {
#ifdef DEBUG
        fprintf(stderr, "%p free :t tile-map\n", *tiles);
#endif

        free(*tiles);
        *tiles = NULL;
        return 0;
    }

#ifdef DEBUG
    fprintf(stderr, "%p malloc :t map-data\n", *data);
#endif

    int len = w * h;

    if(map == NULL) {
        memset(*data, 0, len);
    } else {
        memcpy(*data, map, len);
    }

    return 1;
}

void tile_map_destroy(TILE_MAP ** tiles) {
    assert(tiles);

    if(!*tiles) {
        return;
    }

    char ** map = &(*tiles)->map;

#ifdef DEBUG
    fprintf(stderr, "%p free :t map-data\n", *map);
#endif

    free(*map);
    *map = NULL;

#ifdef DEBUG
    fprintf(stderr, "%p free :t tile-map\n", *tiles);
#endif

    free(*tiles);
    *tiles = NULL;
}

char tile_map_get(TILE_MAP * tiles, int x, int y) {
    assert(tiles);
    assert(tiles->map);

    char * map = tiles->map;

    return tile_map_get_aux(map, tiles->size.w, x, y);
}

char tile_map_get_aux(const char * const map, int w, int x, int y) {
    assert(map);

    int i = TILE_MAP_ADDRESS(w, x, y);

    return map[i];
}

char tile_map_set(TILE_MAP * tiles, int x, int y, char flag) {
    assert(tiles);
    assert(tiles->map);

    char * map = tiles->map;

    return tile_map_set_aux(map, tiles->size.w, x, y, flag);
}

char tile_map_set_aux(char * const map, int w, int x, int y, char flag) {
    assert(map);

    int i = TILE_MAP_ADDRESS(w, x, y);
    char old = map[i];

    map[i] = flag;

    return old;
}
