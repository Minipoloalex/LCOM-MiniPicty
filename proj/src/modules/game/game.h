#pragma once

#include <lcom/lcf.h>
#include "player_drawer/player_drawer.h"
#include "canvas/canvas.h"

#include "../../model/state/state.h"
#include "../../model/position/position.h"
#include "../../model/brush/brush.h"
#include "../../model/player/player.h"
#include "../../model/button/button.h"
#include "../../model/guess/guess.h"
#include "../../model/prompt/prompt.h"

#include "../../devices/keyboard/keyboard.h"
#include "../../devices/mouse/mouse.h"
#include "../../devices/serial_port/serial_port.h"
#include "../../devices/video_card/video_card.h"

int (setup_game)(bool isTransmitter);
void (destroy_game)();

int (game_process_keyboard)();
int (game_process_mouse)();
int (game_process_serial)();
int (game_draw)();
void (transitionToGame)(state_t* state);

void (update_cursor_state)(position_t position);
