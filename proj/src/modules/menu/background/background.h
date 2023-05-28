#pragma once

#include <lcom/lcf.h>
#include "../../../devices/video_card/video_card.h"

/** @defgroup background Background
 * @{
 *
 * @brief Module responsible for the background of the menu
 */

typedef uint8_t Background;

/**
 * @brief Create a new Background
 * 
 * @return Background* 
 */
Background * (create_background)();
/**
 * @brief Destroy a Background
 * 
 * @param background_scene 
 */
void (destroy_background)(Background* background_scene);

/**
 * @brief Draw the background to the buffer based on the hour
 * 
 * @param background_scene buffer to draw the background
 * @param hour a number between 0 and 23 that represents the hour of the day
 */
void (draw_timelapse)(Background* background_scene, int hour);
