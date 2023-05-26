#pragma once
#include <lcom/lcf.h>

#include "../position/position.h"
#include "../../devices/video_card/video_card.h"

typedef struct {
  position_t position;
  uint8_t x_speed;
  uint8_t y_speed;
  
  xpm_image_t *xpms;
  int number_xpms;
  int current_xpm;
  int aspeed;
} asteroid_t;

/**
 * @brief 
 * 
 */
asteroid_t *(create_asteroid)(uint8_t number_of_xpms, xpm_map_t *xpms, uint8_t x_speed, uint8_t y_speed, int aspeed);
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
