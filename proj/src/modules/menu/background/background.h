#pragma once

#include <lcom/lcf.h>
#include "../../../devices/video_card/video_card.h"

typedef uint8_t Background;

/**
 * @brief 
 * 
 */
Background * (create_background)();
/**
 * @brief 
 * 
 */
void (destroy_background)(Background* background_scene);
/**
 * @brief 
 * 
 */
void (draw_timelapse)(Background* background_scene, int hour);
