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
  Event process_timer;
  ButtonsGetter get_buttons;
  bool running_app;
};

/**
 * @brief 
 * 
 * @param state 
 */
void default_implementation(state_t* state);
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
