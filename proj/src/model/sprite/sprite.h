#pragma once

#include <lcom/lcf.h>

typedef struct {
  int width, height;
  uint8_t *colors;
} Sprite;

/**
 * @brief Create a new Sprite
 * 
 * @param xpm 
 * @return Sprite* 
 */
Sprite * (create_sprite)(xpm_map_t xpm);

/**
 * @brief Destroy a Sprite
 * 
 * @param sprite 
 */
void (destroy_sprite)(Sprite *sprite);
