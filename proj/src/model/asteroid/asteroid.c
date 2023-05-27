#include "asteroid.h"

asteroid_t *(create_asteroid)(Sprite *xpms[]) {
  asteroid_t *asteroid = malloc(sizeof(asteroid_t));
  if (asteroid == NULL) {
    return NULL;
  }
  asteroid->xpms = xpms;
  if (asteroid->xpms == NULL) {
    free(asteroid);
    return NULL;
  }
  asteroid->aspeed = ASTEROID_ASPEED;
  asteroid_reset_position(asteroid);
  return asteroid;
}

void (destroy_asteroid)(asteroid_t *asteroid) {
  free(asteroid);
}

bool (is_inside)(asteroid_t *asteroid, position_t position){
  Sprite *xpm = asteroid->xpms[asteroid->current_xpm];
  uint8_t *colors = xpm->colors;
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

int (draw_asteroid)(asteroid_t *asteroid) {
  if (vg_draw_sprite(asteroid->xpms[asteroid->current_xpm], asteroid->position.x, asteroid->position.y)) {
    printf("vg_draw_sprite inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
void (asteroid_reset_position)(asteroid_t *asteroid) {
  asteroid->position = (position_t) {
    .x = get_h_res()/2,
    .y = 100
  };
  asteroid->current_xpm = ASTEROID_INITIAL_XPM;
  asteroid->x_speed = ASTEROID_INITIAL_X_SPEED;
  asteroid->y_speed = ASTEROID_INITIAL_Y_SPEED;
}
