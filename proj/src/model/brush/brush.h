#pragma once

#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>

#define MIN_BRUSH_SIZE 1
#define MAX_BRUSH_SIZE 5

struct brush {
  uint16_t color;
  uint16_t size;
};
typedef struct brush brush_t;

brush_t *(create_brush)(uint16_t color, uint16_t size);
void (destroy_brush)(brush_t *brush);
void (set_brush_color)(brush_t *brush, uint16_t color);
void (brush_increase_size)(brush_t *brush);
void (brush_decrease_size)(brush_t *brush);
