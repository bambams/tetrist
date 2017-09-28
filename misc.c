#include "misc.h"

ALLEGRO_COLOR rgb_to_color(RGB rgb) {
    return al_map_rgb(rgb.r, rgb.g, rgb.b);
}
