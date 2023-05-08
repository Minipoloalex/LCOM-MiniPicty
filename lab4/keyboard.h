#pragma once
#include <stdint.h>
#include <stdbool.h>

#include <lcom/lcf.h>

#include "i8042.h"
#include "kbc.h"

int (keyboard_subscribe_interrupts)(uint8_t *bit_no);
int (keyboard_unsubscribe_interrupts)();
void (keyboard_ih)();
int (keyboard_enable_int)();
