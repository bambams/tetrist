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

    #include "tile_map.h"

ALLEGRO_COLOR rgb_to_color(RGB);

#endif
