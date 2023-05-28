#pragma once

#include <lcom/lcf.h>

/** @defgroup position Position
 * @{
 *
 * @brief Module responsible for utility functions related to positions on the screen
 */

/**
 * @brief Position struct
 * 
 */
typedef struct position {
  uint16_t x;
  uint16_t y;
} position_t;

/**
 * @brief Drawing position
 * This struct is used to store the position of the mouse and if it is drawing/clicking or not
 */
typedef struct drawing_position {
  position_t position;
  bool is_drawing;
} drawing_position_t;

/**
 * @brief 
 * @param bytes There should be 4 bytes corresponding to:\n
 * byte 0 - lsb x\n
 * byte 1 - msb x\n
 * byte 2 - lsb y\n
 * byte 3 - msb y
 * @param position return parameter position
 * @return int 0 if success, 1 otherwise
 */
int (get_position)(uint8_t bytes[4], position_t *position);

/**
 * @brief Checks if a position is inside an area/rectangle defined by a start point, width and height
 * 
 * @param position position to check if it is inside the area/rectangle
 * @param start_point top left point of the area/rectangle
 * @param width width of the area/rectangle
 * @param height height of the area/rectangle
 * @return bool - true if the position is inside the area, false otherwise
 */
bool (is_inside_rectangle)(position_t position, position_t start_point, uint16_t width, uint16_t height);
