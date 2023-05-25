#pragma once


struct State;
typedef struct State state_t;

typedef int (*Event)(state_t*);

struct State{
  Event draw;
  Event process_mouse;
  Event process_keyboard;
  Event process_serial;
  Event process_timer;
};

void defaultImplementation(state_t* state);
