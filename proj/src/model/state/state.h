#pragma once

#include <lcom/lcf.h>
#include "../button/button.h"

/** @defgroup state State
 * @{
 *
 * @brief Module responsible for the state
 */

struct State;
typedef struct State state_t;

typedef int (*Event)(state_t*);
typedef buttons_array_t* (*ButtonsGetter)(state_t*);

/**
 * @brief State struct
 * Contains all the functions that will be called in the main loop in each state
 * 
*/
struct State {
  Event draw;
  Event process_mouse;
  Event process_keyboard;
  Event process_serial;
  Event process_timer;
  Event process_rtc;
  ButtonsGetter get_buttons;
  bool running_app;
  uint8_t word_index;
};

/**
 * @brief Set default functions in all the State fields
 * 
 * @param state 
 */
void default_implementation(state_t* state);

/**
 * @brief Create a new State
 * 
 */
state_t *(create_state)();

/**
 * @brief Destroy a state
 * 
 * @param state 
 */
void (destroy_state)(state_t *state);
