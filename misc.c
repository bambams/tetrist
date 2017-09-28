#include "misc.h"

int collision_detected(POINT p1, TILE_MAP * t1,
                              POINT p2, TILE_MAP * t2) {
    SIZE s1 = t1->size;
    SIZE s2 = t2->size;
    int mx1 = p1.x + s1.w;
    int mx2 = p2.x + s2.w;
    int my1 = p1.y + s1.h;
    int my2 = p2.y + s2.h;

    for(int x1 = p1.x; x1 < mx1; x1++) {
        for(int y1 = p1.y; y1 < my1; y1++) {
            for(int x2 = p2.x; x2 < mx2; x2++) {
                for(int y2 = p2.y; y2 < my2; y2++) {
                    if(x1 == x2 && y1 == y2 &&
                            tile_map_get(t1, x1, y1) &&
                            tile_map_get(t2, x2, y2)) {
                        return 1;
                    }
                }
            }
        }
    }

    return 0;
}

ALLEGRO_COLOR rgb_to_color(RGB rgb) {
    return al_map_rgb(rgb.r, rgb.g, rgb.b);
}
