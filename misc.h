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
#ifndef MISC_H
    #define MISC_H

    #include <allegro5/allegro.h>

struct POINT {
    int x, y;
};

struct RGB {
    int r, g, b;
};

struct SIZE {
    int w, h;
};

typedef struct POINT POINT;
typedef struct RGB RGB;
typedef struct SIZE SIZE;

typedef void (*FUNCTION_DESTROY)(void **);

    #include "tile_map.h"

char collision_detected(POINT, TILE_MAP *, POINT, TILE_MAP *, POINT *);
void free_nil(void **);
ALLEGRO_COLOR rgb_to_color(RGB);

#endif
