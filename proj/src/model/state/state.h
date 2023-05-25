#pragma once

#include <lcom/lcf.h>

struct State;
typedef struct State state_t;

typedef int (*Event)(state_t*);

struct State{
  Event draw;
  Event process_mouse;
  Event process_keyboard;
  Event process_serial;
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
