#include <lcom/lcf.h>

#include "player_menu/player_menu.h"
#include "background/background.h"

#include "../game/game.h"
#include "../resources/resources.h"
#include "../../model/state/state.h"
#include "../../devices/video_card/video_card.h"
#include "../../devices/mouse/mouse.h"
#include "../../devices/serial_port/serial_port.h"
#include "../../devices/rtc/rtc.h"

#include "../../model/button/button.h"

/** @defgroup menu Menu
 * @{
 *
 * @brief Module responsible for the menu screen
 */

/**
 * @brief Setup the menu by creating the necessary objects
 * 
 * @param state Reference to the app state object
 * @param resources Reference to an already loaded resources object
 * @return int 0 if success, 1 otherwise
 */
int (setup_menu)(state_t *state, Resources *resources);

/**
 * @brief Update the player menu position
 * 
 * @param state Reference to the app state object
 * @return int 0 if success, 1 otherwise
 */
int (update_player_menu)();

/**
 * @brief Draw the menu
 * 
 * @param state Reference to the app state object
 * @return int 0 if success, 1 otherwise
 */
int (menu_draw)();
/**
 * @brief Handle the mouse interrupts inside the menu state
 * 
 * @param state Reference to the app state object
 * @return int 0 if success, 1 otherwise
 */
int (menu_process_mouse)();
/**
 * @brief Handle the serial port interrupts inside the menu state
 * 
 * @param state Reference to the app state object
 * @return int 0 if success, 1 otherwise
 */
int (menu_process_serial)();
/**
 * @brief Handle the rtc interrupts inside the menu state
 * 
 * @param state Reference to the app state object
 * @return int 0 if success, 1 otherwise
 */
int (menu_process_rtc)();
/**
 * @brief Get the buttons array object
 * 
 * @param state Reference to the app state object
 * @return buttons_array_t*
 */
buttons_array_t *(menu_get_buttons)(state_t *state);

/**
 * @brief Transition to the menu state by changing the state functions
 * 
 * @param state Reference to the app state object
 */
void (transition_to_menu)(state_t* state);

/**
 * @brief Destroy the menu by freeing the necessary objects
 * 
 */
void (destroy_menu)();
