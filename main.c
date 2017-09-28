#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <stdlib.h>

#include "list.h"

#define black (al_map_rgb(0, 0, 0))
#define blue (al_map_rgb(0, 0, 255))
#define cyan (al_map_rgb(0, 255, 255))
#define dgray (al_map_rgb(50, 50, 50))
#define gray (al_map_rgb(75, 75, 75))
#define green (al_map_rgb(0, 255, 0))
#define lgray (al_map_rgb(100, 100, 100))
#define magicpink (al_map_rgb(255, 0, 255))
#define orange (al_map_rgb(255, 165, 0))
#define purple (al_map_rgb(128, 0, 128))
#define red (al_map_rgb(255, 0, 0))
#define white (al_map_rgb(255, 255, 255))
#define yellow (al_map_rgb(255, 255, 0))
#define pink (al_map_rgb(254, 0, 254))

const int GFX_FPS = 10;
const int GRAVITY = 1;
const int LOGIC_FPS = 1;
const int TILE_SIZE = 40;

#define _XT(x) (x * TILE_SIZE)
#define _0T 0
#define _1T (_XT(1))
#define _2T (_XT(2))
#define _3T (_XT(3))
#define _4T (_XT(4))
#define _5T (_XT(5))

typedef enum {
    PIECE_I,
    PIECE_J,
    PIECE_L,
    PIECE_O,
    PIECE_S,
    PIECE_T,
    PIECE_Z,
    NUM_PIECES
} GAME_PIECE_TYPE;

typedef struct {
    int x, y;
} POINT;

typedef struct {
    int w, h;
} SIZE;

typedef struct {
    ALLEGRO_BITMAP * sprite;
    GAME_PIECE_TYPE type;
    POINT position;
} GAME_PIECE;

typedef struct {
    int quit;
    int status;

    ALLEGRO_DISPLAY * display;
    ALLEGRO_EVENT_QUEUE * events;
    ALLEGRO_TIMER * gfx_timer;
    ALLEGRO_TIMER * logic_timer;

    struct {
        ALLEGRO_BITMAP * block;
        ALLEGRO_BITMAP * game_board;
        ALLEGRO_BITMAP * pieces[7];
    } sprites;

    LINKED_LIST * pieces;
} GAME_STATE;

static void apply_gravity(LINKED_LIST **);
static int create_block(ALLEGRO_BITMAP **, ALLEGRO_COLOR);
static int create_block_shaded(ALLEGRO_BITMAP **, ALLEGRO_COLOR,
                        ALLEGRO_COLOR, ALLEGRO_COLOR);
static int create_game_board(GAME_STATE *);
static int create_piece_i(ALLEGRO_BITMAP **);
static int create_piece_j(ALLEGRO_BITMAP **);
static int create_piece_l(ALLEGRO_BITMAP **);
static int create_piece_o(ALLEGRO_BITMAP **);
static int create_piece_s(ALLEGRO_BITMAP **);
static int create_piece_t(ALLEGRO_BITMAP **);
static int create_piece_z(ALLEGRO_BITMAP **);
static int create_sprite(ALLEGRO_BITMAP **, int, int);
static int deinitialize(GAME_STATE *);
static void draw_block(ALLEGRO_BITMAP *, int, int);
static void draw_pieces(LINKED_LIST **);
static int get_x(int reset);
static int get_y(int reset);
static int initialize(GAME_STATE *);
static int initialize_pieces(GAME_STATE *);
static void piece_destroy(GAME_PIECE **);
static GAME_PIECE * piece_spawn(GAME_STATE *, GAME_PIECE_TYPE);
static void process_logic(GAME_STATE *);
static void render_graphics(GAME_STATE *);

int main(int argc, char * argv[])
{
    int redraw = 1;
    GAME_STATE S;

    S.status = initialize(&S);

    if(S.status != 0) {
        goto exit;
    }

    if(!initialize_pieces(&S)) {
        S.status = 1;
    }

    if(S.status != 0) {
        goto exit;
    }

    al_start_timer(S.gfx_timer);
    al_start_timer(S.logic_timer);

    while(!S.quit) {
        ALLEGRO_EVENT ev;

        al_wait_for_event(S.events, &ev);

        ALLEGRO_TIMER * source = NULL;

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
                source = ev.timer.source;

                if(source == S.gfx_timer && redraw) {
                    redraw = 0;
                    render_graphics(&S);
                } else if(source == S.logic_timer) {
                    process_logic(&S);
                    redraw = 1;
                }
                break;
        }
    }

exit:
    S.status = deinitialize(&S);

    return S.status;
}

static void apply_gravity(LINKED_LIST ** pieces) {
    LINKED_LIST * list = *pieces;

    while(list != NULL) {
        GAME_PIECE * piece = list->data;

        piece->position.y += GRAVITY * TILE_SIZE;

        list = list->next;
    }
}

static int create_block(ALLEGRO_BITMAP ** sprite, ALLEGRO_COLOR fill) {
    return create_block_shaded(sprite, fill, lgray, dgray);
}

static int create_block_shaded(ALLEGRO_BITMAP ** sprite,
                               ALLEGRO_COLOR fill,
                               ALLEGRO_COLOR topleft,
                               ALLEGRO_COLOR bottomright) {
    if(!create_sprite(sprite, 1, 1)) {
        return 0;
    }

    al_clear_to_color(fill);
    al_draw_line(_1T - 4/2, _0T, _1T - 4/2, _1T, bottomright, 4);
    al_draw_line(_1T, _1T - 4/2, _0T, _1T - 4/2, bottomright, 4);
    al_draw_line(_0T, _0T + 4/2, _1T, _0T + 4/2, topleft, 4);
    al_draw_line(_0T + 4/2, _1T, _0T + 4/2, _0T, topleft, 4);

    return 1;
}

static int create_game_board(GAME_STATE * S) {
    int w = al_get_display_width(S->display);
    int h = al_get_display_height(S->display);
    int tw = w / TILE_SIZE;
    int th = h / TILE_SIZE;
    ALLEGRO_BITMAP ** sprite = &S->sprites.game_board;

    if(!create_sprite(sprite, tw, th)) {
        return 0;
    }

    ALLEGRO_BITMAP * block = NULL;

    if(!create_block(&block, gray)) {
        al_destroy_bitmap(*sprite);
        *sprite = NULL;
        return 0;
    }

    al_set_target_bitmap(*sprite);

    for(int y=0; y<th; y++) {
        for(int x=0; x<tw; x++) {
            if(x == 0 || x == tw - 1 || y == 0 || y == th - 1) {
                al_draw_bitmap(block, x * TILE_SIZE, y * TILE_SIZE, 0);
            }
        }
    }

    return 1;
}

static int create_piece_i(ALLEGRO_BITMAP ** sprite) {
    if(!create_sprite(sprite, 4, 1)) {
        return 0;
    }

    ALLEGRO_BITMAP * block = NULL;

    if(!create_block(&block, cyan)) {
        al_destroy_bitmap(*sprite);
        *sprite = NULL;
        return 0;
    }

    al_set_target_bitmap(*sprite);
    draw_block(block, 0, 0);
    draw_block(block, 1, 0);
    draw_block(block, 2, 0);
    draw_block(block, 3, 0);

    return 1;
}

static int create_piece_j(ALLEGRO_BITMAP ** sprite) {
    if(!create_sprite(sprite, 3, 2)) {
        return 0;
    }

    ALLEGRO_BITMAP * block = NULL;

    if(!create_block(&block, blue)) {
        al_destroy_bitmap(*sprite);
        *sprite = NULL;
        return 0;
    }

    al_set_target_bitmap(*sprite);
    draw_block(block, 0, 0);
    draw_block(block, 1, 0);
    draw_block(block, 2, 0);
    draw_block(block, 2, 1);

    return 1;
}

static int create_piece_l(ALLEGRO_BITMAP ** sprite) {
    if(!create_sprite(sprite, 3, 2)) {
        return 0;
    }

    ALLEGRO_BITMAP * block = NULL;

    if(!create_block(&block, orange)) {
        al_destroy_bitmap(*sprite);
        *sprite = NULL;
        return 0;
    }

    al_set_target_bitmap(*sprite);
    draw_block(block, 0, 0);
    draw_block(block, 1, 0);
    draw_block(block, 2, 0);
    draw_block(block, 0, 1);

    return 1;
}

static int create_piece_o(ALLEGRO_BITMAP ** sprite) {
    if(!create_sprite(sprite, 2, 2)) {
        return 0;
    }

    ALLEGRO_BITMAP * block = NULL;

    if(!create_block(&block, yellow)) {
        al_destroy_bitmap(*sprite);
        *sprite = NULL;
        return 0;
    }

    al_set_target_bitmap(*sprite);
    draw_block(block, 0, 0);
    draw_block(block, 1, 0);
    draw_block(block, 0, 1);
    draw_block(block, 1, 1);

    return 1;
}

static int create_piece_s(ALLEGRO_BITMAP ** sprite) {
    if(!create_sprite(sprite, 3, 2)) {
        return 0;
    }

    ALLEGRO_BITMAP * block = NULL;

    if(!create_block(&block, green)) {
        al_destroy_bitmap(*sprite);
        *sprite = NULL;
        return 0;
    }

    al_set_target_bitmap(*sprite);
    draw_block(block, 1, 0);
    draw_block(block, 2, 0);
    draw_block(block, 0, 1);
    draw_block(block, 1, 1);

    return 1;
}

static int create_piece_t(ALLEGRO_BITMAP ** sprite) {
    if(!create_sprite(sprite, 3, 2)) {
        return 0;
    }

    ALLEGRO_BITMAP * block = NULL;

    if(!create_block(&block, purple)) {
        al_destroy_bitmap(*sprite);
        *sprite = NULL;
        return 0;
    }

    al_set_target_bitmap(*sprite);
    draw_block(block, 0, 0);
    draw_block(block, 1, 0);
    draw_block(block, 2, 0);
    draw_block(block, 1, 1);

    return 1;
}

static int create_piece_z(ALLEGRO_BITMAP ** sprite) {
    if(!create_sprite(sprite, 3, 2)) {
        return 0;
    }

    ALLEGRO_BITMAP * block = NULL;

    if(!create_block(&block, red)) {
        al_destroy_bitmap(*sprite);
        *sprite = NULL;
        return 0;
    }

    al_set_target_bitmap(*sprite);
    draw_block(block, 0, 0);
    draw_block(block, 1, 0);
    draw_block(block, 1, 1);
    draw_block(block, 2, 1);

    return 1;
}

static int create_sprite(ALLEGRO_BITMAP ** sprite, int w, int h)
{
    assert(sprite);

    *sprite = al_create_bitmap(_XT(w), _XT(h));

    if(*sprite) {
        al_set_target_bitmap(*sprite);
        al_clear_to_color(magicpink);
        al_convert_mask_to_alpha(*sprite, magicpink);
    }

    return *sprite != NULL;
}

static int deinitialize(GAME_STATE * S)
{
    int i, l;
    ALLEGRO_BITMAP ** sprite = NULL;
    ALLEGRO_DISPLAY ** display = &S->display;
    ALLEGRO_EVENT_QUEUE ** events = &S->events;
    ALLEGRO_TIMER ** gfx_timer = &S->gfx_timer;
    ALLEGRO_TIMER ** logic_timer = &S->logic_timer;
    LINKED_LIST ** pieces = &S->pieces;

    list_destroy(pieces, (void (*)(void **))piece_destroy);

    sprite = S->sprites.pieces;

    for(i=0,l=6; i<l; i++) {
        if(*sprite) {
            al_destroy_bitmap(*sprite);
            *sprite++ = NULL;
        }
    }

    if(*events) {
        al_destroy_event_queue(*events);
        *events = NULL;
    }

    if(*gfx_timer) {
        al_destroy_timer(*gfx_timer);
        *gfx_timer = NULL;
    }

    if(*logic_timer) {
        al_destroy_timer(*logic_timer);
        *logic_timer = NULL;
    }

    if(*display) {
        al_destroy_display(*display);
        *display = NULL;
    }

    return S->status;
}

static void draw_block(ALLEGRO_BITMAP * block, int x, int y) {
    al_draw_bitmap(block, _XT(x), _XT(y), 0);
}

static void draw_pieces(LINKED_LIST ** pieces) {
    LINKED_LIST * list = *pieces;

    while(list != NULL) {
        GAME_PIECE * piece = list->data;

        al_draw_bitmap(piece->sprite,
                       piece->position.x, piece->position.y,
                       0);

        list = list->next;
    }
}

static int get_x(int reset) {
    #define DEFX 40
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
    #define DEFY 40
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

static int initialize(GAME_STATE * S)
{
    memset(S, 0, sizeof(GAME_STATE));

    S->quit = 0;

    ALLEGRO_BITMAP ** sprite = NULL;
    ALLEGRO_DISPLAY ** display = &S->display;
    ALLEGRO_EVENT_QUEUE ** events = &S->events;
    ALLEGRO_TIMER ** gfx_timer = &S->gfx_timer;
    ALLEGRO_TIMER ** logic_timer = &S->logic_timer;

    if(!al_init()) {
        return 1;
    }

    if(!al_install_keyboard()) {
        return 2;
    }

    *display = al_create_display(480, 640);

    if(*display == NULL) {
        return 3;
    }

    *gfx_timer = al_create_timer(1.0/GFX_FPS);

    if(*gfx_timer == NULL) {
        return 4;
    }

    *logic_timer = al_create_timer(1.0/LOGIC_FPS);

    if(*logic_timer == NULL) {
        return 5;
    }

    *events = al_create_event_queue();

    if(*events == NULL) {
        return 6;
    }

    al_register_event_source(*events, al_get_display_event_source(*display));
    al_register_event_source(*events, al_get_keyboard_event_source());
    al_register_event_source(*events, al_get_timer_event_source(*gfx_timer));
    al_register_event_source(*events, al_get_timer_event_source(*logic_timer));

    if(!al_init_primitives_addon()) {
        return 7;
    }

    sprite = &S->sprites.block;

    if(!create_block(sprite, pink)) {
        return 8;
    }

    sprite = &S->sprites.game_board;

    if(!create_game_board(S)) {
        return 8;
    }

    sprite = S->sprites.pieces;

    if(!create_piece_i(sprite)) {
        return 10;
    }

    if(!create_piece_j(++sprite)) {
        return 11;
    }

    if(!create_piece_l(++sprite)) {
        return 12;
    }

    if(!create_piece_o(++sprite)) {
        return 13;
    }

    if(!create_piece_s(++sprite)) {
        return 14;
    }

    if(!create_piece_t(++sprite)) {
        return 15;
    }

    if(!create_piece_z(++sprite)) {
        return 16;
    }

    return 0;
}

static int initialize_pieces(GAME_STATE * S) {
    LINKED_LIST ** pieces = &S->pieces;

    get_x(1);
    get_y(1);

    for(GAME_PIECE_TYPE i=PIECE_I; i<NUM_PIECES; i++) {
        GAME_PIECE * piece = piece_spawn(S, i);

        piece->position.x = get_x(0);
        piece->position.y = get_y(0);

        if(piece == NULL || !list_add(pieces, piece)) {
            goto error;
        }
    }

    return 1;

error:
    list_destroy(pieces, (void (*)(void **))piece_destroy);
    return 0;
}

static void piece_destroy(GAME_PIECE ** piece) {
    if(*piece != NULL) {
        free(*piece);
        *piece = NULL;
    }
}

static GAME_PIECE * piece_spawn(GAME_STATE * S, GAME_PIECE_TYPE type) {
    assert(type >= 0);
    assert(type < NUM_PIECES);

    GAME_PIECE * piece = malloc(sizeof(GAME_PIECE));

    if(piece == NULL) {
        return NULL;
    }

    memset(piece, 0, sizeof(GAME_PIECE));

    ALLEGRO_BITMAP * sprite = S->sprites.pieces[type];

    int bx = al_get_bitmap_width(sprite);
    int dx = al_get_display_width(S->display);
    int x = dx / 2 - bx / 2;

    piece->position.x = x;
    piece->sprite = sprite;
    piece->type = type;

    return piece;
}

static void process_logic(GAME_STATE * S) {
    apply_gravity(&S->pieces);
}

static void render_graphics(GAME_STATE * S) {
    al_set_target_bitmap(al_get_backbuffer(S->display));
    al_clear_to_color(white);
    al_draw_bitmap(S->sprites.game_board, 0, 0, 0);
    al_draw_bitmap(S->sprites.block, get_x(1), get_y(1), 0);
    draw_pieces(&S->pieces);
    al_flip_display();
}
