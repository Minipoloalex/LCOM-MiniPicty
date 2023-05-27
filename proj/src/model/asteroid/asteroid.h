#pragma once
#include <lcom/lcf.h>

#include "../position/position.h"
#include "../../devices/video_card/video_card.h"
#include "../../modules/resources/asteroids.h"

#define ASTEROID_INITIAL_X_SPEED 1
#define ASTEROID_INITIAL_Y_SPEED 1
#define ASTEROID_INITIAL_XPM DOWN_RIGHT
#define ASTEROID_ASPEED 1

typedef struct {
  position_t position;
  int8_t x_speed;
  int8_t y_speed;

  Sprite **xpms;
  asteroid_type_t current_xpm;
  int aspeed;
} asteroid_t;

/**
 * @brief 
 * 
 */
asteroid_t *(create_asteroid)(Sprite *xpms[]);
/**
 * @brief 
 * 
 */
void (destroy_asteroid)(asteroid_t *asteroid);
/**
 * @brief 
 * 
 */
bool (is_inside)(asteroid_t *asteroid, position_t position);
/**
 * @brief 
 * 
 */
int (draw_asteroid)(asteroid_t *asteroid);

void (asteroid_reset_position)(asteroid_t *asteroid);
