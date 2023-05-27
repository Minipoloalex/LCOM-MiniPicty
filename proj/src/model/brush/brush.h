#pragma once

#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>

#define MIN_BRUSH_SIZE 1  /**< @brief minimum size of the circles drawn by the brush */
#define MAX_BRUSH_SIZE 25  /**< @brief maximum size of the circles drawn by the brush*/
/**
 * @brief Brush struct. Has a color and size.
 */
struct brush {
  uint32_t color;
  uint16_t size;
};
typedef struct brush brush_t;
/**
 * @brief Creates a brush with a given color and size.
 * @param color color of the created brush
 * @param size size of the created brush
 */
brush_t *(create_brush)(uint32_t color, uint16_t size);
/**
 * @brief Destroys a brush.
 */
void (destroy_brush)(brush_t *brush);
/**
 * @brief Sets the color of a brush.
 */
void (set_brush_color)(brush_t *brush, uint32_t color);
/**
 * @brief Increases the size of a brush by 2 pixels
 */
void (brush_increase_size)(brush_t *brush);
/**
 * @brief Decreases the size of a brush by 2 pixels
 */
void (brush_decrease_size)(brush_t *brush);
