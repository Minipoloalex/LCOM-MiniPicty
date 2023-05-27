#pragma once

#include <lcom/lcf.h>

typedef struct {
  int x, y;
  int width, height;
  uint8_t *colors;
} Sprite;

Sprite * (create_sprite)(xpm_map_t xpm, uint16_t x, uint16_t y);

void (destroy_sprite)(Sprite *sprite);
