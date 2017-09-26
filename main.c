#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <stdio.h>
#include <stdlib.h>

const int FPS = 10;
const int TILE_SIZE = 40;

enum {
    PIECE_I,
    PIECE_J,
    PIECE_L,
    PIECE_O,
    PIECE_S,
    PIECE_T,
    PIECE_Z,
    NUM_PIECES
};

typedef struct {
    ALLEGRO_DISPLAY * display;
    ALLEGRO_EVENT_QUEUE * events;
    ALLEGRO_TIMER * timer;

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

    al_destroy_event_queue(S->events);
    al_destroy_timer(S->timer);
    al_destroy_display(S->display);
}

void initialize(GAME_STATE * S)
{
    ALLEGRO_BITMAP ** sprite = NULL;
    ALLEGRO_DISPLAY ** display = &S->display;
    ALLEGRO_EVENT_QUEUE ** events = &S->events;
    ALLEGRO_TIMER ** timer = &S->timer;

    if(!al_init()) {
        exit(1);
    }

    if(!al_install_keyboard()) {
        exit(2);
    }

    *display = al_create_display(480, 640);

    if(*display == NULL) {
        exit(3);
    }

    *timer = al_create_timer(1.0/FPS);

    *events = al_create_event_queue();

    if(*events == NULL) {
        exit(4);
    }

    al_register_event_source(*events, al_get_keyboard_event_source());
    al_register_event_source(*events, al_get_timer_event_source(*timer));

    sprite = S->sprites.pieces;

    if(!create_piece_i(sprite)) {
        exit(5);
    }

    if(!create_piece_j(sprite)) {
        exit(6);
    }

    if(!create_piece_l(sprite)) {
        exit(7);
    }

    if(!create_piece_o(sprite)) {
        exit(8);
    }

    if(!create_piece_s(sprite)) {
        exit(9);
    }

    if(!create_piece_t(sprite)) {
        exit(10);
    }

    if(!create_piece_z(sprite)) {
        exit(11);
    }
}

int create_sprite(ALLEGRO_BITMAP ** sprite, int w, int h)
{
    assert(sprite);

    *sprite = al_create_bitmap(TILE_SIZE * w, TILE_SIZE * h);

    return *sprite != NULL;
}

int create_piece_i(ALLEGRO_BITMAP ** sprite) {
    if(!create_sprite(++sprite, 4, 1)) {
        exit(5);
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
