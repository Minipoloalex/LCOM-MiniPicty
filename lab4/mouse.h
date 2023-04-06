#pragma once
#include <stdint.h>
#include <lcom/lcf.h>
#include "i8042.h"
#include "kbc.h"

int (mouse_subscribe_interrupts)(uint8_t *bit_no);
int (mouse_unsubscribe_interrupts)();
int (mouse_enable_data_report)();
int (mouse_disable_data_report)();
void (mouse_ih)();
