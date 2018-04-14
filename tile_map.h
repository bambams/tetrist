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

int map_to_string(char *, char **, int);
int tile_map_create(TILE_MAP **, int, int, const char * const);
void tile_map_destroy(TILE_MAP **);
char tile_map_get(TILE_MAP *, int, int);
char tile_map_get_aux(const char * const, int, int, int);
char tile_map_set(TILE_MAP *, int, int, char);
char tile_map_set_aux(char * const, int, int, int, char);
const char const * tile_map_string(TILE_MAP *, char **, char *);

#endif
