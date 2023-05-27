#pragma once

#include <lcom/lcf.h>

typedef struct {
  int x, y;
  int width, height;
  uint8_t *colors;
} Sprite;

Sprite * (create_sprite)(xpm_map_t xpm);

void (destroy_sprite)(Sprite *sprite);
