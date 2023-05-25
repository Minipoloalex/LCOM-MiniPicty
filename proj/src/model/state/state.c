#include "state.h"

static int default_int_returner(state_t* state){
  return 0;
}
static buttons_array_t *(default_get_buttons)(state_t* state){
  return NULL;
}

void default_implementation(state_t* state){
  state->draw = default_int_returner;
  state->process_mouse = default_int_returner;
  state->process_keyboard = default_int_returner;
  state->process_serial = default_int_returner;
  state->process_timer = default_int_returner;
  state->get_buttons = default_get_buttons;
}

state_t *(create_state)() {
  return malloc(sizeof(state_t));
}

void (destroy_state)(state_t *state) {
  free(state);
}
