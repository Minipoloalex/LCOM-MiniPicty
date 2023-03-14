#pragma once
#include <stdint.h>
#include "i8042.h"
#include "kbc.h"

int (mouse_subscribe_interrupts)(uint8_t *bit_no);
int (mouse_unsubscribe_interrupts)();
int (mouse_disable_data_reporting)();
void (mouse_ih)();
