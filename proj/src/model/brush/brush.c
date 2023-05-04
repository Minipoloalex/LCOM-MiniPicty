#include "brush.h"

struct brush {
  uint16_t color;
  uint16_t size;
  bool drawing;
};

bool (is_drawing)(brush_t *brush) {
  return brush->drawing;
}
int (set_drawing)(brush_t *brush, bool drawing) {
  if (brush == NULL) {
    printf("set_drawing: brush is NULL\n");
    return EXIT_FAILURE;
  }
  brush->drawing = drawing;
  return EXIT_SUCCESS;
}
