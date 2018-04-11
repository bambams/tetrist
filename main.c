#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "misc.h"
#include "tile_map.h"

#define black (al_map_rgb(0, 0, 0))
#define dgray (al_map_rgb(50, 50, 50))
#define gray (al_map_rgb(75, 75, 75))
#define lgray (al_map_rgb(100, 100, 100))
#define magicpink (al_map_rgb(255, 0, 255))
#define white (al_map_rgb(255, 255, 255))

const int GRAVITY = 1;
const int LOGIC_FPS = 5;
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

typedef enum {
    HORIZONTAL = 1,
    VERTICAL = 2
} INPUT_DIRECTION;

typedef struct {
    ALLEGRO_BITMAP * sprite;
    POINT spawn;
    SIZE bitmap_size;
    SIZE tile_size;
    TILE_MAP * tiles;
} GAME_BOARD;

typedef struct {
    int moving;
    int noclip;
    ALLEGRO_BITMAP * sprite;
    GAME_PIECE_TYPE type;
    POINT next_position;
    POINT position;
    TILE_MAP * tiles;
} GAME_PIECE;

typedef struct {
    char tile;
    int game_board_collision;
    int player_fail;
    GAME_PIECE * piece;
    POINT origin;
    POINT spot;
    TILE_MAP * tiles;
} COLLISION;

typedef struct {
    int move_down;
    int move_left;
    int move_right;
} PLAYER;

typedef struct {
    int down;
    int game_over;
    int quit;
    int redraw;
    int respawn;
    int status;
    int ticks;

    ALLEGRO_DISPLAY * display;
    ALLEGRO_EVENT_QUEUE * events;
    ALLEGRO_TIMER * timer;
    GAME_BOARD * game_board;
    GAME_PIECE * current_piece;
    LINKED_LIST * pieces;
    PLAYER player;

    struct {
        ALLEGRO_BITMAP * game_board;
        ALLEGRO_BITMAP * pieces[7];
    } sprites;
} GAME_STATE;

static void apply_input(GAME_STATE *, INPUT_DIRECTION);
static void apply_gravity(GAME_STATE *);
static void apply_movements(GAME_STATE *);
static void collision_destroy(COLLISION **);
static COLLISION * collision_spawn(GAME_PIECE *, POINT *, POINT *,
                                   TILE_MAP *, char, int, int);
static int create_block(ALLEGRO_BITMAP **, ALLEGRO_COLOR);
static int create_block_shaded(ALLEGRO_BITMAP **, ALLEGRO_COLOR,
                        ALLEGRO_COLOR, ALLEGRO_COLOR);
static int create_game_board(GAME_STATE *);
static int create_piece_sprite(ALLEGRO_BITMAP **, GAME_PIECE_TYPE);
static int create_sprite(ALLEGRO_BITMAP **, int, int);
static int deinitialize(GAME_STATE *);
static int detect_collisions(GAME_STATE *, GAME_PIECE *, LINKED_LIST **);
static void draw_block(ALLEGRO_BITMAP *, int, int);
static void draw_pieces(LINKED_LIST **);
static void game_board_destroy(GAME_BOARD **);
static GAME_BOARD * game_board_spawn(GAME_STATE *);
static int initialize(GAME_STATE *);
static int initialize_game_board(GAME_STATE *);
static int map_to_string(char *, char **, int);
static GAME_PIECE_TYPE next_piece_type(GAME_STATE *);
static void piece_destroy(GAME_PIECE **);
static GAME_PIECE * piece_spawn(GAME_STATE *, GAME_PIECE_TYPE);
static void print_collision(COLLISION *);
static void print_frame_diagnostics(GAME_STATE *);
static int process_logic(GAME_STATE *);
static void render_graphics(GAME_STATE *);
static void reset_movement(GAME_STATE *);
static int spawn_next_piece(GAME_STATE *);

static const RGB piece_colors[] = {
    // I
    {0, 255, 255},
    // J
    {0, 0, 255},
    // L
    {255, 165, 0},
    // O
    {255, 255, 0},
    // S
    {0, 255, 0},
    // T
    {128, 0, 128},
    // Z
    {255, 0, 0},
};

static const SIZE piece_sizes[] = {
    // I
    {4,1},
    // J
    {3,2},
    // L
    {3,2},
    // O
    {2,2},
    // S
    {3,2},
    // T
    {3,2},
    // Z
    {3,2}
};

static const char * const piece_tiles[] = {
    // I
    "xxxx",
    // J
    "xxx"
    "\0\0x",
    // L
    "xxx"
    "x\0\0",
    // O
    "xx"
    "xx",
    // S
    "\0xx"
    "xx\0",
    // T
    "xxx"
    "\0x\0",
    // Z
    "xx\0"
    "\0xx"
};

static const char type_names[] = {
    'I','J','L','O', 'S','T','Z'
};

int main(int argc, char * argv[])
{
    GAME_STATE S;

    S.status = initialize(&S);

    if(S.status != 0) {
        goto exit;
    }

    if(!initialize_game_board(&S)) {
        S.status = 1;
        goto exit;
    }

    if(!spawn_next_piece(&S)) {
        S.status = 1;
        goto exit;
    }

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
            case ALLEGRO_EVENT_KEY_UP:
                S.down = ev.type == ALLEGRO_EVENT_KEY_DOWN;

                switch(ev.keyboard.keycode) {
                    case ALLEGRO_KEY_ESCAPE:
                    case ALLEGRO_KEY_Q:
                        S.quit = 1;
                        break;
                    case ALLEGRO_KEY_H:
                        S.player.move_left = S.down;
                        break;
                    case ALLEGRO_KEY_J:
                        S.player.move_down = S.down;
                        break;
                    case ALLEGRO_KEY_L:
                        S.player.move_right = S.down;
                        break;
                }
                break;
            case ALLEGRO_EVENT_TIMER:
                process_logic(&S);

                if(S.redraw) {
                    render_graphics(&S);
                }

                break;
        }
    }

exit:
    S.status = deinitialize(&S);

    return S.status;
}

static void apply_input(GAME_STATE * S, INPUT_DIRECTION direction) {
    GAME_PIECE * current_piece = S->current_piece;
    PLAYER * player = &S->player;
    POINT * next = NULL;

    if(current_piece == NULL) {
        return;
    }

    next = &current_piece->next_position;

    if(current_piece->noclip) {
        return;
    }

    int horizontal = direction & HORIZONTAL;
    int vertical = direction & VERTICAL;

    fprintf(stderr, "Applying input: %s%s%s\n", vertical && player->move_down ? "J" : "", horizontal && player->move_left ? "H" : "", horizontal && player->move_right ? "L" : "");

    if(horizontal) {
        if(player->move_left) {
            next->x -= 1;
        }

        if(player->move_right) {
            next->x += 1;
        }
    }

    if(vertical) {
        if(player->move_down) {
            next->y += 1;
        }
    }
}

static void apply_gravity(GAME_STATE * S) {
    LINKED_LIST * list = S->pieces;

    while(list != NULL) {
        GAME_PIECE * piece = list->data;
        POINT * next = &piece->next_position;

        next->y += GRAVITY;
        piece->moving = 1;

        list = list->next;
    }
}

static void apply_movements(GAME_STATE * S) {
    GAME_PIECE * current_piece = S->current_piece;
    LINKED_LIST * list = S->pieces;

    while(list != NULL) {
        GAME_PIECE * piece = list->data;

        LINKED_LIST * collisions = NULL;

        if(detect_collisions(S, piece, &collisions)) {
            while(collisions != NULL) {
                COLLISION * collision = collisions->data;

                print_collision(collision);

                char tile = collision->tile;

                if(piece == current_piece && tile == 'x') {
                    S->respawn = 1;
                }

                collisions = collisions->next;
            }
        } else {
            fprintf(stderr,
                    "No collision detected. "
                    "Applying movement from (%d,%d) to (%d,%d).\n",
                    piece->position.x, piece->position.y,
                    piece->next_position.x, piece->next_position.y);

            piece->noclip = 0;
            piece->position = piece->next_position;
            piece->moving = 0;

            S->redraw = 1;
        }

        list = list->next;
    }
}

static void collision_destroy(COLLISION ** collision) {
    assert(collision);

    if(*collision == NULL) {
        return;
    }

    free(*collision);
    *collision = NULL;
}

static COLLISION * collision_spawn(GAME_PIECE * piece,
                                    POINT * origin,
                                    POINT * spot,
                                    TILE_MAP * tiles,
                                    char tile,
                                    int game_board_collision,
                                    int player_fail) {
    COLLISION * collision = malloc(sizeof(COLLISION));

    if(collision == NULL) {
        return NULL;
    }

    collision->game_board_collision = 1;
    collision->origin = *origin;
    collision->piece = piece;
    collision->player_fail = player_fail;
    collision->spot = *spot;
    collision->tile = tile;
    collision->tiles = tiles;

    return collision;
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
    int x, y;

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

    for(y=0; y<th; y++) {
        for(x=0; x<tw; x++) {
            if(x == 0 || x == tw - 1 || y == 0 || y == th - 1) {
                al_draw_bitmap(block, x * TILE_SIZE, y * TILE_SIZE, 0);
            }
        }
    }

    return 1;
}

static int create_piece_sprite(ALLEGRO_BITMAP ** sprite,
                               GAME_PIECE_TYPE type) {
    assert(type >= 0);
    assert(type < NUM_PIECES);

    const char * const map = piece_tiles[type];
    const RGB * const rgb = &piece_colors[type];
    const SIZE * const size = &piece_sizes[type];
    int w = size->w;
    int h = size->h;
    int x;
    int y;

    if(!create_sprite(sprite, w, h)) {
        return 0;
    }

    ALLEGRO_BITMAP * block = NULL;
    ALLEGRO_COLOR color = rgb_to_color(*rgb);

    if(!create_block(&block, color)) {
        al_destroy_bitmap(*sprite);
        *sprite = NULL;
        return 0;
    }

    al_set_target_bitmap(*sprite);

    for(y=0; y<h; y++) {
        for(x=0; x<w; x++) {
            if(tile_map_get_aux(map, w, x, y)) {
                draw_block(block, x, y);
            }
        }
    }

    al_destroy_bitmap(block);
    block = NULL;

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
    ALLEGRO_TIMER ** timer = &S->timer;
    GAME_BOARD ** game_board = &S->game_board;
    LINKED_LIST ** pieces = &S->pieces;

    list_destroy(pieces, (FUNCTION_DESTROY)piece_destroy);
    game_board_destroy(game_board);

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

    if(*timer) {
        al_destroy_timer(*timer);
        *timer = NULL;
    }

    if(*display) {
        al_destroy_display(*display);
        *display = NULL;
    }

    return S->status;
}

static int detect_collisions(GAME_STATE * S, GAME_PIECE * piece,
                             LINKED_LIST ** collisions) {
    assert(*collisions == NULL);

    COLLISION * collision = NULL;
    GAME_BOARD * game_board = S->game_board;
    GAME_PIECE * current_piece = S->current_piece;
    LINKED_LIST * list = S->pieces;
    POINT p1 = piece->next_position;
    POINT p2 = {0,0};
    TILE_MAP * t1 = piece->tiles;
    TILE_MAP * t2 = game_board->tiles;
    POINT spot;
    char detected = collision_detected(p1, t1, p2, t2, &spot);
    int * noclip = &piece->noclip;

    if(detected) {
        if(*noclip) {
            fprintf(stderr,
                    "Ignoring collision with game board because the "
                    "piece is noclipped.\n");
        } else {
            collision = collision_spawn(piece, &p2, &spot, t2,
                                        detected, 1,
                                        current_piece == piece);

            if(collision == NULL) {
                return 0;
            }

            if(!list_add(collisions, collision)) {
                collision_destroy(&collision);
                return 0;
            }
	     }
    }

    while(list != NULL) {
        GAME_PIECE * other_piece = list->data;

        if(piece != other_piece) {
            p2 = other_piece->position;
            t2 = other_piece->tiles;

            detected = collision_detected(p1, t1, p2, t2, &spot);

            if(detected) {
                collision = collision_spawn(piece, &p2, &spot, t2,
                                            detected, 0,
                                            current_piece == piece);

                if(collision == NULL) {
                    list_destroy(collisions,
                                 (FUNCTION_DESTROY)collision_destroy);
                    return 0;
                }

                if(!list_add(collisions, collision)) {
                    collision_destroy(&collision);
                    list_destroy(collisions,
                                 (FUNCTION_DESTROY)collision_destroy);
                    return 0;
                }
            }
        }

        list = list->next;
    }

    return *collisions != NULL;
}

static void draw_block(ALLEGRO_BITMAP * block, int x, int y) {
    al_draw_bitmap(block, _XT(x), _XT(y), 0);
}

static void draw_pieces(LINKED_LIST ** pieces) {
    LINKED_LIST * list = *pieces;

    while(list != NULL) {
        GAME_PIECE * piece = list->data;

        al_draw_bitmap(piece->sprite,
                       _XT(piece->position.x), _XT(piece->position.y),
                       0);

        list = list->next;
    }
}

static void game_board_destroy(GAME_BOARD ** game_board) {
    assert(game_board);

    if(!*game_board) {
        return;
    }

    TILE_MAP ** tiles = &(*game_board)->tiles;

    tile_map_destroy(tiles);

    free(*game_board);
    *game_board = NULL;
}

static GAME_BOARD * game_board_spawn(GAME_STATE * S) {
    int x, y;
    GAME_BOARD * game_board = malloc(sizeof(GAME_BOARD));

    if(game_board == NULL) {
        return NULL;
    }

    memset(game_board, 0, sizeof(GAME_BOARD));

    int w = al_get_display_width(S->display);
    int wt = w / TILE_SIZE;
    int h = al_get_display_height(S->display);
    int ht = h / TILE_SIZE;
    ALLEGRO_BITMAP * sprite = S->sprites.game_board;
    POINT * spawn = &game_board->spawn;
    SIZE * bitmap_size = &game_board->bitmap_size;
    SIZE * tile_size = &game_board->tile_size;
    TILE_MAP ** tiles = &game_board->tiles;

    if(!tile_map_create(tiles, wt, ht, NULL)) {
        free(game_board);
        game_board = NULL;
        return NULL;
    }

    for(y=0; y<ht; y++) {
        for(x=0; x<wt; x++) {
            if(x == 0 || x == wt - 1 || y == 0 || y == ht - 1) {
                tile_map_set(*tiles, x, y, y == ht - 1 ? 'x' : 'w');
            }
        }
    }

    bitmap_size->w = w;
    bitmap_size->h = h;
    spawn->x = wt / 2;
    spawn->y = 0;
    tile_size->w = wt;
    tile_size->h = ht;

    game_board->sprite = sprite;

    return game_board;
}

static int initialize(GAME_STATE * S)
{
    memset(S, 0, sizeof(GAME_STATE));

    S->redraw = 1;

    ALLEGRO_BITMAP ** sprite = NULL;
    ALLEGRO_DISPLAY ** display = &S->display;
    ALLEGRO_EVENT_QUEUE ** events = &S->events;
    ALLEGRO_TIMER ** timer = &S->timer;
    GAME_PIECE_TYPE i;

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

    *timer = al_create_timer(1.0/LOGIC_FPS);

    if(*timer == NULL) {
        return 4;
    }

    *events = al_create_event_queue();

    if(*events == NULL) {
        return 6;
    }

    al_register_event_source(*events, al_get_display_event_source(*display));
    al_register_event_source(*events, al_get_keyboard_event_source());
    al_register_event_source(*events, al_get_timer_event_source(*timer));

    if(!al_init_primitives_addon()) {
        return 7;
    }

    sprite = &S->sprites.game_board;

    if(!create_game_board(S)) {
        return 8;
    }

    sprite = S->sprites.pieces;

    for(i=PIECE_I; i<NUM_PIECES; i++) {
        if(!create_piece_sprite(&sprite[i], i)) {
            return 9;
        }
    }

    return 0;
}

static int initialize_game_board(GAME_STATE * S) {
    GAME_BOARD ** game_board = &S->game_board;

    *game_board = game_board_spawn(S);

    if(game_board == NULL) {
        return 0;
    }

    return 1;
}

static int map_to_string(char * src, char ** dest, int len) {
    char c;
    int i;

    *dest = malloc(len + 1);

    if(*dest == NULL) {
        return 0;
    }

    for(i=0; i<len; i++) {
        c = src[i];

        (*dest)[i] = c == '\0' ? '0' : c;
    }

    (*dest)[len] = '\0';

    return 1;
}

static GAME_PIECE_TYPE next_piece_type(GAME_STATE * S) {
    static GAME_PIECE_TYPE type = PIECE_Z;

    type = (type + 1) % NUM_PIECES;

    return type;
}

static void piece_destroy(GAME_PIECE ** piece) {
    assert(piece);

    if(!*piece) {
        return;
    }

    TILE_MAP ** tiles = &(*piece)->tiles;

    tile_map_destroy(tiles);

    free(*piece);
    *piece = NULL;
}

static GAME_PIECE * piece_spawn(GAME_STATE * S, GAME_PIECE_TYPE type) {
    assert(type >= 0);
    assert(type < NUM_PIECES);

    GAME_BOARD * game_board = S->game_board;
    POINT * spawn = &game_board->spawn;
    GAME_PIECE * piece = malloc(sizeof(GAME_PIECE));
    LINKED_LIST ** pieces = &S->pieces;

    if(piece == NULL) {
        return NULL;
    }

    memset(piece, 0, sizeof(GAME_PIECE));

    const char * const map = piece_tiles[type];
    const SIZE * const size = &piece_sizes[type];
    TILE_MAP ** tiles = &piece->tiles;

    if(!tile_map_create(tiles, size->w, size->h, map)) {
        free(piece);
        piece = NULL;
        return NULL;
    }

    ALLEGRO_BITMAP * sprite = S->sprites.pieces[type];
    int w = size->w;

    piece->noclip = 1;
    piece->position.x = spawn->x - w / 2;
    piece->position.y = spawn->y;
    piece->sprite = sprite;
    piece->type = type;

    if(!list_add(pieces, piece)) {
        tile_map_destroy(tiles);
        free(piece);
        piece = NULL;
        return NULL;
    }

    S->current_piece = piece;

    return piece;
}

static void print_collision(COLLISION * collision) {
    int game_board_collision = collision->game_board_collision;
    int player_fail = collision->player_fail;
    GAME_PIECE * piece = collision->piece;
    POINT * p2 = &collision->origin;
    TILE_MAP * t2 = collision->tiles;
    POINT * spot = &collision->spot;
    POINT * p1 = &piece->next_position;
    TILE_MAP * t1 = piece->tiles;
    SIZE s1 = t1->size;
    SIZE s2 = t2->size;
    int len1 = sizeof(char) * s1.w * s1.h;
    int len2 = sizeof(char) * s2.w * s2.h;
    char * map1 = "<error>";
    char * map2 = "<error>";
    int map1b = map_to_string(t1->map, &map1, len1);
    int map2b = map_to_string(t2->map, &map2, len2);

    fprintf(stderr,
            "Piece (0x%p) is colliding at (%d,%d) with %s. "
            "Piece is at (%d,%d) with tile map \"%s\" "
            "and colliding piece is at (%d,%d) with tile map "
            "\"%s\". %s\n",
            piece,
            spot->x, spot->y,
            game_board_collision ? "the game board" : "another piece",
            p1->x, p1->y,
            map1,
            p2->x, p2->y,
            map2,
            player_fail ?
            "Respawning a new piece." :
            "Piece stopped.");

    if(map1b) free(map1);
    if(map2b) free(map2);
}

static void print_frame_diagnostics(GAME_STATE * S) {
    LINKED_LIST * list = S->pieces;

    fprintf(stderr, "FRAME RESULT: pieces:");

    while(list != NULL) {
        GAME_PIECE * piece = list->data;

        char * map = "<error>";
        int mapb = map_to_string(piece->tiles->map, &map, piece->tiles->size.w * piece->tiles->size.h);

        fprintf(stderr, " '(%c (%d %d))",
                         type_names[piece->type],
                         piece->position.x,
                         piece->position.y);

        if(mapb) free(map);

        list = list->next;
    }

    fputc('\n', stderr);
}

static int process_logic(GAME_STATE * S) {
    S->ticks++;

    apply_input(S, VERTICAL);
    apply_gravity(S);
    apply_movements(S);

    apply_input(S, HORIZONTAL);
    apply_movements(S);

    if(S->respawn) {
        S->respawn = 0;

        if(!spawn_next_piece(S)) {
            return 0;
        }
    }

    print_frame_diagnostics(S);

    // Reset movement for next frame.
    reset_movement(S);

    return 1;
}

static void render_graphics(GAME_STATE * S) {
    S->redraw = 0;
    al_set_target_bitmap(al_get_backbuffer(S->display));
    al_clear_to_color(white);
    al_draw_bitmap(S->game_board->sprite, 0, 0, 0);
    draw_pieces(&S->pieces);
    al_flip_display();
}

static void reset_movement(GAME_STATE * S) {
    LINKED_LIST * list = S->pieces;

    while(list != NULL) {
        GAME_PIECE * piece = list->data;

        piece->next_position = piece->position;

        list = list->next;
    }
}

static int spawn_next_piece(GAME_STATE * S) {
    GAME_PIECE_TYPE type = next_piece_type(S);
    GAME_PIECE * piece = piece_spawn(S, type);
    LINKED_LIST * collisions = NULL;

    piece->next_position = piece->position;

    if (detect_collisions(S, piece, &collisions)) {
        S->current_piece = NULL;
        list_remove(&S->pieces, piece);
        piece_destroy(&piece);
        S->game_over = 1;
    }

    return piece != NULL;
}
