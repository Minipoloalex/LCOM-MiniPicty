#include "asteroid.h"

asteroid_t *(create_asteroid)(uint8_t number_of_xpms, xpm_map_t *xpms, uint8_t x_speed, uint8_t y_speed, int aspeed) {
  asteroid_t *asteroid = malloc(sizeof(asteroid_t));
  if (asteroid == NULL) {
    return NULL;
  }
  asteroid->xpms = malloc(sizeof(xpm_image_t) * number_of_xpms);
  if (asteroid->xpms == NULL) {
    free(asteroid);
    return NULL;
  }
  asteroid->number_xpms = number_of_xpms;
  asteroid->current_xpm = 0;
  asteroid->x_speed = x_speed;
  asteroid->y_speed = y_speed;
  asteroid->aspeed = aspeed;
  // load xpms
  return asteroid;
}

void (destroy_asteroid)(asteroid_t *asteroid) {
  free(asteroid->xpms);
  free(asteroid);
}

bool (is_inside)(asteroid_t *asteroid, position_t position){
  xpm_image_t *xpm = &asteroid->xpms[asteroid->current_xpm];
  uint8_t *colors = xpm->bytes;
  position_t asteroid_position = asteroid->position;
  if (!is_inside_rectangle(position, asteroid_position, xpm->width, xpm->height)) {
    return false;
  }
  uint16_t asteroid_x = asteroid->position.x;
  uint16_t asteroid_y = asteroid->position.x;  
  for (int row = asteroid_y; row < asteroid_y + xpm->height; row++) {
    for (int col = asteroid_x; col < asteroid_x + xpm->width; col++, colors += get_bytes_per_pixel()) {
      if (col != position.x || row != position.y) {
        continue;
      }
      return *colors != xpm_transparency_color(XPM_8_8_8_8);
    }
  }
  return false;
}
