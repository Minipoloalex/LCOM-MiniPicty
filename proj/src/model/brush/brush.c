#include "brush.h"

brush_t *(create_brush)(uint32_t color, uint16_t size) {
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

void (set_brush_color)(brush_t *brush, uint32_t color) {
  brush->color = color;
}

void (brush_increase_size)(brush_t *brush) {
  if(brush->size + BRUSH_STEP <= MAX_BRUSH_SIZE) brush->size = brush->size + BRUSH_STEP;
}

void (brush_decrease_size)(brush_t *brush) {
  if(brush->size - BRUSH_STEP > MIN_BRUSH_SIZE) brush->size = brush->size - BRUSH_STEP;
}
