#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <lcom/lcf.h>

#include "../controllers/i8042.h"
#include "../controllers/kbc/kbc.h"
#include "../../model/position.h"
#include "../video_card/video_card.h"


bool (packet_is_ready)();
uint8_t *(mouse_get_packet_bytes)();

int (mouse_subscribe_interrupts)(uint8_t *bit_no);
int (mouse_unsubscribe_interrupts)();
int (mouse_enable_data_report)();
int (mouse_disable_data_report)();
void (mouse_ih)();
int (mouse_process_packet)();
int (write_to_mouse)(uint8_t command);
int (mouse_enable_int)();
int (mouse_disable_int)();
int (mouse_get_packet)();
