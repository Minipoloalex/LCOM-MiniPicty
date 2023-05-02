#pragma once
#include <lcom/lcf.h>
#include "brush.h"
#include "position.h"

typedef enum {
  SELF_PLAYER,
  OTHER_PLAYER,
} player_type_t;

struct PlayerDrawer {
  position_t mouse_position;
  player_type_t player_state;
  brush_t brush;
};
struct PlayerGuesser {
  position_t mouse_position;
  player_type_t player_state;
};
typedef struct PlayerDrawer PlayerDrawer_t;
typedef struct PlayerGuesser PlayerGuesser_t;
