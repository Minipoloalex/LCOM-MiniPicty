#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <lcom/lcf.h>

#include "../controllers/i8042.h"
#include "../controllers/kbc/kbc.h"
#include "../video_card/video_card.h"
#include "../../model/drawing_position.h"

/**
 * @brief 
 * 
 */
bool (packet_is_ready)();
/**
 * @brief 
 * 
 */
uint8_t *(mouse_get_packet_bytes)();
/**
 * @brief 
 * 
 */
int (mouse_process_packet_byte)();
/**
 * @brief 
 * 
 */
void (mouse_ih)();
/**
 * @brief 
 * Always call this if and only if packet_is_ready() returns true
 */
drawing_position_t (mouse_get_drawing_position_from_packet)(position_t before_position);

/**
 * @brief 
 * 
 */
int (mouse_subscribe_interrupts)(uint8_t *bit_no);
/**
 * @brief 
 * 
 */
int (mouse_unsubscribe_interrupts)();
/**
 * @brief 
 * 
 */
int (mouse_enable_int)();
/**
 * @brief 
 * 
 */
int (mouse_disable_int)();
/**
 * @brief 
 * 
 */
int (mouse_enable_data_report)();
/**
 * @brief 
 * 
 */
int (mouse_disable_data_report)();
