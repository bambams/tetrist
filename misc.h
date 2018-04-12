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
