#include "tetrist.h"

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
