#include "misc.h"

char collision_detected(POINT p1, TILE_MAP * t1,
                       POINT p2, TILE_MAP * t2,
                       POINT * spot) {
    SIZE s1 = t1->size;
    SIZE s2 = t2->size;

    for(int x1 = 0; x1 < s1.w; x1++) {
        for(int y1 = 0; y1 < s1.h; y1++) {
            int c1 = tile_map_get(t1, x1, y1);

            if(!c1) {
                continue;
            }

            int bx = x1 + p1.x;
            int by = y1 + p1.y;
            int x2 = bx - p2.x;
            int y2 = by - p2.y;

            if(x2 < 0 || x2 > s2.w - 1 || y2 < 0 || y2 > s2.h - 1) {
                continue;
            }

            int c2 = tile_map_get(t2, x2, y2);

            if(c2) {
                spot->x = bx;
                spot->y = by;

                return c2;
            }
        }
    }

    return 0;
}

ALLEGRO_COLOR rgb_to_color(RGB rgb) {
    return al_map_rgb(rgb.r, rgb.g, rgb.b);
}
