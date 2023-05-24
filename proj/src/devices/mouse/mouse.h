#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <lcom/lcf.h>

#include "../controllers/i8042.h"
#include "../controllers/kbc/kbc.h"
#include "../video_card/video_card.h"
#include "../../model/drawing_position.h"

bool (packet_is_ready)();
uint8_t *(mouse_get_packet_bytes)();
int (mouse_process_packet)();
int (mouse_process_packet_byte)();
void (mouse_ih)();
/**
 * @brief 
 * Always call this if and only if packet_is_ready() returns true
 */
drawing_position_t (mouse_get_drawing_position_from_packet)(position_t before_position);

// TODO: make some functions private (lower level like write_to_mouse)
// An idea: make a mouse init/config and a mouse_destroy
int (mouse_subscribe_interrupts)(uint8_t *bit_no);
int (mouse_unsubscribe_interrupts)();
int (mouse_enable_int)();
int (mouse_disable_int)();
int (mouse_enable_data_report)();
int (mouse_disable_data_report)();
int (write_to_mouse)(uint8_t command);
