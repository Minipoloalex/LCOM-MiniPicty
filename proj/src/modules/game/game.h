#pragma once

#include <lcom/lcf.h>

#include "player_drawer/player_drawer.h"
#include "canvas/canvas.h"
#include "../menu/menu.h"
#include "../resources/resources.h"
#include "../../model/state/state.h"
#include "../../model/position/position.h"
#include "../../model/brush/brush.h"
#include "../../model/player/player.h"
#include "../../model/button/button.h"
#include "../../model/guess/guess.h"
#include "../../model/prompt/prompt.h"
#include "../../model/asteroid/asteroid.h"

#include "../../devices/timer/timer.h"
#include "../../devices/keyboard/keyboard.h"
#include "../../devices/mouse/mouse.h"
#include "../../devices/serial_port/serial_port.h"
#include "../../devices/video_card/video_card.h"
#include "../../devices/rtc/rtc.h"
#include "../../utils/utils.h"

/** @defgroup game Game
 * @{
 *
 * @brief Module responsible for the game logic
 */

/**
 * @brief Setup the game by creating the necessary objects
 * 
 * @param isTransmitter True if the player is the transmitter, false otherwise
 * @param state Reference to the app state object
 * @param resources Reference to an already loaded resources object
 * @return int 0 if success, 1 otherwise
 */
int (setup_game)(bool isTransmitter, state_t *state, Resources* resources);

/**
 * @brief Destroy the game by freeing the necessary objects
 * 
 */
void (destroy_game)();

/** 
 * @brief Draw the game
 * @return int 0 if success, 1 otherwise
 */
int (game_draw)();
/**
 * @brief Handle the timer interrupts inside the game state
 * 
 * @return int 0 if success, 1 otherwise
 */
int (game_process_timer)();
/**
 * @brief Handle the keyboard interrupts inside the game state
 * 
 * @return int 0 if success, 1 otherwise
 */
int (game_process_keyboard)();
/**
 * @brief Handle the mouse interrupts inside the game state
 * 
 * @return int 0 if success, 1 otherwise
 */
int (game_process_mouse)();
/**
 * @brief Handle the serial port interrupts inside the game state
 * 
 * @return int 0 if success, 1 otherwise
 */
int (game_process_serial)();
/**
 * @brief Get the buttons array object
 * 
 * @param state Reference to the app state object
 * @return buttons_array_t* Pointer to the buttons array
 */
buttons_array_t *(game_get_buttons)(state_t* state);
/**
 * @brief Transition to the game state by changing the state functions
 * 
 * @param state Reference to the app state object
 */
void (transition_to_game)(state_t* state, bool hard_mode);

/**
 * @brief Update the cursor state
 * 
 * @param position Position of the cursor
 */
void (update_cursor)(position_t position);

/**
 * @brief Update the asteroid position and check if it collides with the canvas
 * 
 * @param asteroid Reference to the asteroid object
 */
int (game_move_asteroid)(asteroid_t *asteroid);
