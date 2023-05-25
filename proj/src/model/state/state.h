#pragma once

#include <lcom/lcf.h>
#include "../button/button.h"

struct State;
typedef struct State state_t;

typedef int (*Event)(state_t*);
typedef buttons_array_t* (*ButtonsGetter)(state_t*);

struct State {
  Event draw;
  Event process_mouse;
  Event process_keyboard;
  Event process_serial;
  ButtonsGetter get_buttons;
};

/**
 * @brief 
 * 
 * @param state 
 */
void defaultImplementation(state_t* state);
/**
 * @brief 
 * 
 */
state_t *(create_state)();

/**
 * @brief 
 * 
 * @param state 
 */
void (destroy_state)(state_t *state);
