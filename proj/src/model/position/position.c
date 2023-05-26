#include "position.h"

int (get_position)(uint8_t bytes[4], position_t *position) {
  if (bytes == NULL || position == NULL) {
    return EXIT_FAILURE;
  }
  position->x = bytes[0] | (bytes[1] << 8);
  position->y = bytes[2] | (bytes[3] << 8);
  return EXIT_SUCCESS;
}
bool (is_inside_rectangle)(position_t position, position_t start_point, uint16_t width, uint16_t height){
  return (
    (start_point.x <= position.x) && (position.x < (start_point.x+width))&&
    (start_point.y <= position.y) && (position.y < (start_point.y+height))
  );
}
