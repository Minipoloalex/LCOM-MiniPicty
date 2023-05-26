#pragma once

#include <lcom/lcf.h>

struct position {
  uint16_t x;
  uint16_t y;
};
typedef struct position position_t;
/**
 * @brief 
 * @param bytes There should be 4 bytes corresponding to:\n
 * byte 0 - lsb x\n
 * byte 1 - msb x\n
 * byte 2 - lsb y\n
 * byte 3 - msb y
 * @param position return parameter position
 */
int (get_position)(uint8_t bytes[4], position_t *position);
/**
 * @brief 
 * 
 */
bool (is_inside_rectangle)(position_t position, position_t start_point, uint16_t width, uint16_t height);
