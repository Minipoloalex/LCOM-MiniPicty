#include "state.h"

static int defaultDraw(state_t* state){
  return 0;
}

void defaultImplementation(state_t* state){
  state->draw = defaultDraw;
  state->process_mouse = defaultDraw;
  state->process_keyboard = defaultDraw;
  state->process_serial = defaultDraw;
}

state_t *(create_state)() {
  return malloc(sizeof(state_t));
}
