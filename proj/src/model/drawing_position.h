#pragma once
#include <lcom/lcf.h>
#include <stdbool.h>
#include "position/position.h"


struct drawing_position {
  position_t position;
  bool is_drawing;
};

typedef struct drawing_position drawing_position_t;
