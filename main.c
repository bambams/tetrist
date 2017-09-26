#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <stdio.h>
#include <stdlib.h>

const int TILE_SIZE = 40;

enum {
    PIECES_I,
    PIECES_J,
    PIECES_L,
    PIECES_O,
    PIECES_S,
    PIECES_T,
    PIECES_Z,
    NUM_PIECES
};

typedef struct {
    ALLEGRO_DISPLAY * display;

    struct {
        ALLEGRO_BITMAP * pieces[6];
    } sprites;
} GAME_STATE;

void deinitialize(GAME_STATE *);
void initialize(GAME_STATE *);
int create_sprite(ALLEGRO_BITMAP **, int, int);
int create_piece_i(ALLEGRO_BITMAP **);
int create_piece_j(ALLEGRO_BITMAP **);
int create_piece_l(ALLEGRO_BITMAP **);
int create_piece_o(ALLEGRO_BITMAP **);
int create_piece_s(ALLEGRO_BITMAP **);
int create_piece_t(ALLEGRO_BITMAP **);
int create_piece_z(ALLEGRO_BITMAP **);

int main(int argc, char * argv[])
{
    GAME_STATE S;

    initialize(&S);

    deinitialize(&S);

    return 0;
}

void deinitialize(GAME_STATE * S)
{
    int i, l;
    ALLEGRO_BITMAP ** sprite = NULL;

    sprite = S->sprites.pieces;

    for(i=0,l=6; i<l; i++) {
        al_destroy_bitmap(*sprite);
        *sprite = NULL;
    }

    al_destroy_display(S->display);
}

void initialize(GAME_STATE * S)
{
    ALLEGRO_BITMAP ** sprite = NULL;
    ALLEGRO_DISPLAY ** display = &S->display;

    if(!al_init()) {
        exit(-1);
    }

    if(!al_install_keyboard()) {
        exit(-2);
    }

    *display = al_create_display(480, 640);

    if(*display == NULL) {
        exit(-3);
    }

    sprite = S->sprites.pieces;

    if(!create_piece_i(sprite)) {
        exit(-4);
    }

    if(!create_piece_j(sprite)) {
        exit(-5);
    }

    if(!create_piece_l(sprite)) {
        exit(-6);
    }

    if(!create_piece_o(sprite)) {
        exit(-7);
    }

    if(!create_piece_s(sprite)) {
        exit(-8);
    }

    if(!create_piece_t(sprite)) {
        exit(-9);
    }

    if(!create_piece_z(sprite)) {
        exit(-10);
    }
}

int create_sprite(ALLEGRO_BITMAP ** sprite, int w, int h)
{
    assert(sprite);

    *sprite = al_create_bitmap(TILE_SIZE * w, TILE_SIZE * h);

    return *sprite == NULL;
}

int create_piece_i(ALLEGRO_BITMAP ** sprite) {
    if(!create_sprite(++sprite, 4, 1)) {
        exit(-5);
    }
}

int create_piece_j(ALLEGRO_BITMAP ** sprite) {
    if(!create_sprite(++sprite, 3, 2)) {
        return 0;
    }

    return 1;
}

int create_piece_l(ALLEGRO_BITMAP ** sprite) {
    if(!create_sprite(++sprite, 3, 2)) {
        return 0;
    }

    return 1;
}

int create_piece_o(ALLEGRO_BITMAP ** sprite) {
    if(!create_sprite(++sprite, 2, 2)) {
        return 0;
    }

    return 1;
}

int create_piece_s(ALLEGRO_BITMAP ** sprite) {
    if(!create_sprite(++sprite, 3, 2)) {
        return 0;
    }

    return 1;
}

int create_piece_t(ALLEGRO_BITMAP ** sprite) {
    if(!create_sprite(sprite, 3, 2)) {
        return 0;
    }

    return 1;
}

int create_piece_z(ALLEGRO_BITMAP ** sprite) {
    if(!create_sprite(sprite, 3, 2)) {
        return 0;
    }

    return 1;
}
