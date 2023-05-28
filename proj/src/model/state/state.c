#include "state.h"


/**
 * @brief Dummy function to replace specific state functions expecting an int return value
 * 
 * @param state - Program's state
 * @return int - Default return value (0)
 */
static int default_int_returner(state_t* state){
  return 0;
}

/**
 * @brief Dummy function to replace specific state functions expecting a pointer return value 
 * 
 * @param state - Program's state
 * @return buttons_array_t* - Default return value (NULL)
 */
static buttons_array_t *(default_get_buttons)(state_t* state){
  return NULL;
}

void default_implementation(state_t* state){
  state->draw = default_int_returner;
  state->process_mouse = default_int_returner;
  state->process_keyboard = default_int_returner;
  state->process_serial = default_int_returner;
  state->process_timer = default_int_returner;
  state->process_rtc = default_int_returner;
  state->get_buttons = default_get_buttons;
}

state_t *(create_state)() {
  state_t *state = malloc(sizeof(state_t));
  state->running_app = true;
  state->word_index = 255;
  return state;
}

void (destroy_state)(state_t *state) {
  free(state);
}
