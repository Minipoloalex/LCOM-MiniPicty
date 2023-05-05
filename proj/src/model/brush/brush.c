#include "brush.h"

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
