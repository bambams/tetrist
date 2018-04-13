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
#include "misc.h"

char collision_detected(POINT p1, TILE_MAP * t1,
                       POINT p2, TILE_MAP * t2,
                       POINT * spot) {
    SIZE s1 = t1->size;
    SIZE s2 = t2->size;
    int bx, by, c1, c2, x1, y1, x2, y2;

    for(x1 = 0; x1 < s1.w; x1++) {
        for(y1 = 0; y1 < s1.h; y1++) {
            c1 = tile_map_get(t1, x1, y1);

            if(!c1) {
                continue;
            }

            bx = x1 + p1.x;
            by = y1 + p1.y;
            x2 = bx - p2.x;
            y2 = by - p2.y;

            if(x2 < 0 || x2 > s2.w - 1 || y2 < 0 || y2 > s2.h - 1) {
                continue;
            }

            c2 = tile_map_get(t2, x2, y2);

            if(c2) {
                spot->x = bx;
                spot->y = by;

                return c2;
            }
        }
    }

    return 0;
}

void free_nil(void ** data) {}

ALLEGRO_COLOR rgb_to_color(RGB rgb) {
    return al_map_rgb(rgb.r, rgb.g, rgb.b);
}
