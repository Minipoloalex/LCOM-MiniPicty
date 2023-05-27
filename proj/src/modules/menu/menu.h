#include <stdio.h>
#include <lcom/lcf.h>

#include "player_menu/player_menu.h"

#include "../game/game.h"
#include "../../model/state/state.h"
#include "../../devices/video_card/video_card.h"
#include "../../devices/mouse/mouse.h"
#include "../../devices/serial_port/serial_port.h"
#include "../../devices/rtc/rtc.h"

#include "../../model/button/button.h"
#include "../../model/cursor/cursor.h"

int (setup_menu)(state_t *state);

int (draw_player_menu)();

int (menu_draw)();
int (menu_process_mouse)();
int (menu_process_serial)();
int (menu_process_rtc)();
void (transition_to_menu)(state_t* state);


void (destroy_menu)();
int (calculate_sun_height)(int hour);

buttons_array_t *(menu_get_buttons)(state_t *state);
