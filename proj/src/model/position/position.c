#include "position.h"

int (get_position)(uint8_t bytes[4], position_t *position) {
  if (bytes == NULL || position == NULL) {
    return EXIT_FAILURE;
  }
  position->x = bytes[0] | (bytes[1] << 8);
  position->y = bytes[2] | (bytes[3] << 8);
  return EXIT_SUCCESS;
}
