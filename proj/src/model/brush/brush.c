#include "brush.h"

struct brush {
  uint16_t color;
  uint16_t size;
};

brush_t *(create_brush)(uint16_t color, uint16_t size) {
  brush_t *brush = malloc(sizeof(brush_t));
  if (brush == NULL) {
    return NULL;
  }
  brush->color = color;
  brush->size = size;
  return brush;
}
void (destroy_brush)(brush_t *brush) {
  free(brush);
}

int (brush_set_color)(brush_t *brush, uint16_t color) {
  brush->color = color;
  return EXIT_SUCCESS;
}
