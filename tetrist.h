#ifndef TETRIST_TETRIST_H
    #define TETRIST_TETRIST_H

    #include <allegro5/allegro.h>
    #include <allegro5/allegro_font.h>
    #include <allegro5/allegro_native_dialog.h>
    #include <allegro5/allegro_primitives.h>
    #include <allegro5/allegro_ttf.h>
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
    #define overlay (al_map_rgba(0, 0, 0, 200))
    #define pink (al_map_rgb(255, 90, 255))
    #define white (al_map_rgb(255, 255, 255))

    #define _XT(x) (x * TILE_SIZE)
    #define _0T 0
    #define _1T (_XT(1))
    #define _2T (_XT(2))
    #define _3T (_XT(3))
    #define _4T (_XT(4))
    #define _5T (_XT(5))
    #define ticks_to_seconds(ticks) (ticks / (double)LOGIC_FPS)

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
    ALLEGRO_FONT * font;
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

void apply_input(GAME_STATE *, INPUT_DIRECTION);
void apply_gravity(GAME_STATE *);
void apply_movements(GAME_STATE *);
void collision_destroy(COLLISION **);
COLLISION * collision_spawn(GAME_PIECE *, POINT *, POINT *,
                                   TILE_MAP *, char, int, int);
int create_block(ALLEGRO_BITMAP **, ALLEGRO_COLOR);
int create_block_shaded(ALLEGRO_BITMAP **, ALLEGRO_COLOR,
                        ALLEGRO_COLOR, ALLEGRO_COLOR);
int create_game_board(GAME_STATE *);
int create_piece_sprite(ALLEGRO_BITMAP **, GAME_PIECE_TYPE);
int create_sprite(ALLEGRO_BITMAP **, int, int);
int deinitialize(GAME_STATE *);
int detect_collisions(GAME_STATE *, GAME_PIECE *, LINKED_LIST **);
void draw_block(ALLEGRO_BITMAP *, int, int);
void draw_pieces(LINKED_LIST **);
void game_board_destroy(GAME_BOARD **);
GAME_BOARD * game_board_spawn(GAME_STATE *);
int initialize(GAME_STATE *);
int initialize_game_board(GAME_STATE *);
int map_to_string(char *, char **, int);
GAME_PIECE_TYPE next_piece_type(GAME_STATE *);
void piece_destroy(GAME_PIECE **);
GAME_PIECE * piece_spawn(GAME_STATE *, GAME_PIECE_TYPE);
void print_collision(COLLISION *);
void print_frame_diagnostics(GAME_STATE *);
int process_logic(GAME_STATE *);
void render_graphics(GAME_STATE *);
void reset_movement(GAME_STATE *);
int spawn_next_piece(GAME_STATE *);

#endif
