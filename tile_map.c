//
// Tetrist is a falling block game.
// Copyright (C) 2018 Brandon McCaig
//
// This file is part of Tetrist.
//
// Tetrist is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// Tetrist is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libbam.  If not, see <http://www.gnu.org/licenses/>.
//
#include <assert.h>
#include <malloc.h>
#include <string.h>

#include "tile_map.h"

int map_to_string(char * src, char ** dest, int len) {
    char c;
    int i;

    *dest = malloc(len + 1);

    if(*dest == NULL) {
        return 0;
    }

#ifdef DEBUG
    fprintf(stderr, "%p malloc :t map_string\n", *dest);
#endif

    for(i=0; i<len; i++) {
        c = src[i];

        (*dest)[i] = c == '\0' ? '0' : c;
    }

    (*dest)[len] = '\0';

    return 1;
}

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

const char const * tile_map_string(TILE_MAP * tiles, char ** buffer1, char * buffer2) {
    map_to_string(tiles->map, buffer1, tiles->size.w * tiles->size.h);
    sprintf(buffer2, "Tile = { size: { %d, %d }, map = '%s' }", tiles->size.w, tiles->size.h, *buffer1);
    return buffer2;
}
