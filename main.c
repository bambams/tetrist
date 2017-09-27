#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <stdlib.h>

#define black (al_map_rgb(0, 0, 0))
#define blue (al_map_rgb(0, 0, 255))
#define cyan (al_map_rgb(0, 255, 255))
#define green (al_map_rgb(0, 255, 0))
#define magicpink (al_map_rgb(255, 0, 255))
#define orange (al_map_rgb(255, 165, 0))
#define purple (al_map_rgb(128, 0, 128))
#define red (al_map_rgb(255, 0, 0))
#define white (al_map_rgb(255, 255, 255))
#define yellow (al_map_rgb(255, 255, 0))

const int FPS = 10;
const int TILE_SIZE = 40;

#define _XT(x) (x * TILE_SIZE)
#define _0T 0
#define _1T (_XT(1))
#define _2T (_XT(2))
#define _3T (_XT(3))
#define _4T (_XT(4))
#define _5T (_XT(5))

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
    int quit;

    ALLEGRO_DISPLAY * display;
    ALLEGRO_EVENT_QUEUE * events;
    ALLEGRO_TIMER * timer;

    struct {
        ALLEGRO_BITMAP * pieces[7];
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

static int get_x(int reset);
static int get_y(int reset);

int main(int argc, char * argv[])
{
    int redraw = 0;
    GAME_STATE S;

    initialize(&S);

    al_start_timer(S.timer);

    while(!S.quit) {
        ALLEGRO_EVENT ev;

        al_wait_for_event(S.events, &ev);

        switch(ev.type)
        {
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                S.quit = 1;
                break;
            case ALLEGRO_EVENT_KEY_DOWN:
                switch(ev.keyboard.keycode) {
                    case ALLEGRO_KEY_ESCAPE:
                    case ALLEGRO_KEY_Q:
                        S.quit = 1;
                        break;
                }
                break;
            case ALLEGRO_EVENT_TIMER:
                redraw = 1;
                break;
        }

        if(redraw) {
            al_set_target_bitmap(al_get_backbuffer(S.display));
            al_clear_to_color(white);
            al_draw_bitmap(S.sprites.pieces[PIECE_I], get_x(1), get_y(1), 0);
            al_draw_bitmap(S.sprites.pieces[PIECE_J], get_x(0), get_y(0), 0);
            al_draw_bitmap(S.sprites.pieces[PIECE_L], get_x(0), get_y(0), 0);
            al_draw_bitmap(S.sprites.pieces[PIECE_O], get_x(0), get_y(0), 0);
            al_draw_bitmap(S.sprites.pieces[PIECE_S], get_x(0), get_y(0), 0);
            al_draw_bitmap(S.sprites.pieces[PIECE_T], get_x(0), get_y(0), 0);
            al_draw_bitmap(S.sprites.pieces[PIECE_Z], get_x(0), get_y(0), 0);
            al_flip_display();
        }
    }

    deinitialize(&S);

    return 0;
}

#define DEFX 40
#define DEFY 40

static int get_x(int reset) {
    static int i = 0;
    static int x = DEFX;

    if(reset) {
        i = 0;
        x = DEFX;
    } else if(++i == 4) {
        x += _5T;
        i = 0;
    }

    return x;
}

static int get_y(int reset) {
    static int i = 0;
    static int y = DEFY;

    if(reset || ++i == 4) {
        i = 0;
        y = DEFY;
    } else {
        y += _3T;
    }

    return y;
}

void deinitialize(GAME_STATE * S)
{
    int i, l;
    ALLEGRO_BITMAP ** sprite = NULL;

    sprite = S->sprites.pieces;

    for(i=0,l=6; i<l; i++) {
        al_destroy_bitmap(*sprite);
        *sprite++ = NULL;
    }

    al_destroy_event_queue(S->events);
    al_destroy_timer(S->timer);
    al_destroy_display(S->display);
}

void initialize(GAME_STATE * S)
{
    memset(S, 0, sizeof(GAME_STATE));

    S->quit = 0;

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

    al_register_event_source(*events, al_get_display_event_source(*display));
    al_register_event_source(*events, al_get_keyboard_event_source());
    al_register_event_source(*events, al_get_timer_event_source(*timer));

    if(!al_init_primitives_addon()) {
        exit(5);
    }

    sprite = S->sprites.pieces;

    if(!create_piece_i(sprite)) {
        exit(6);
    }

    if(!create_piece_j(++sprite)) {
        exit(7);
    }

    if(!create_piece_l(++sprite)) {
        exit(8);
    }

    if(!create_piece_o(++sprite)) {
        exit(9);
    }

    if(!create_piece_s(++sprite)) {
        exit(10);
    }

    if(!create_piece_t(++sprite)) {
        exit(11);
    }

    if(!create_piece_z(++sprite)) {
        exit(12);
    }
}

int create_sprite(ALLEGRO_BITMAP ** sprite, int w, int h)
{
    assert(sprite);

    *sprite = al_create_bitmap(_XT(w), _XT(h));

    if(*sprite) {
        al_set_target_bitmap(*sprite);
        al_clear_to_color(magicpink);
    }

    return *sprite != NULL;
}

int create_piece_i(ALLEGRO_BITMAP ** sprite) {
    if(!create_sprite(sprite, 4, 1)) {
        return 0;
    }

    al_draw_filled_rectangle(_0T, _0T,
                             _4T, _1T,
                             cyan);

    return 1;
}

int create_piece_j(ALLEGRO_BITMAP ** sprite) {
    if(!create_sprite(sprite, 3, 2)) {
        return 0;
    }

    al_draw_filled_rectangle(_0T, _0T,
                             _3T, _1T,
                             blue);
    al_draw_filled_rectangle(_2T, _1T,
                             _2T + _1T, _1T + _1T,
                             blue);

    return 1;
}

int create_piece_l(ALLEGRO_BITMAP ** sprite) {
    if(!create_sprite(sprite, 3, 2)) {
        return 0;
    }

    al_draw_filled_rectangle(_0T, _0T,
                             _3T, _1T,
                             orange);
    al_draw_filled_rectangle(_0T, _1T,
                             _0T +_1T, _1T + _1T,
                             orange);

    return 1;
}

int create_piece_o(ALLEGRO_BITMAP ** sprite) {
    if(!create_sprite(sprite, 2, 2)) {
        return 0;
    }

    al_draw_filled_rectangle(_0T, _0T,
                             _2T, _2T,
                             yellow);

    return 1;
}

int create_piece_s(ALLEGRO_BITMAP ** sprite) {
    if(!create_sprite(sprite, 3, 2)) {
        return 0;
    }

    al_draw_filled_rectangle(_1T, _0T,
                             _1T + _2T, _1T,
                             green);
    al_draw_filled_rectangle(_0T, _1T,
                             _0T + _2T, _1T + _1T,
                             green);

    return 1;
}

int create_piece_t(ALLEGRO_BITMAP ** sprite) {
    if(!create_sprite(sprite, 3, 2)) {
        return 0;
    }

    al_draw_filled_rectangle(_0T, _0T,
                             _3T, _1T,
                             purple);
    al_draw_filled_rectangle(_1T, _1T,
                             _1T +_1T, _1T + _1T,
                             purple);

    return 1;
}

int create_piece_z(ALLEGRO_BITMAP ** sprite) {
    if(!create_sprite(sprite, 3, 2)) {
        return 0;
    }

    al_draw_filled_rectangle(_0T, _0T,
                             _0T + _2T, _1T,
                             red);
    al_draw_filled_rectangle(_1T, _1T,
                             _1T + _2T, _1T + _1T,
                             red);

    return 1;
}
