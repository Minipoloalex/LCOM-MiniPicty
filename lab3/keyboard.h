#pragma once
#include <lcom/lcf.h>

#include <stdint.h>

#include "kbc.h"

#include "i8042.h"

void (kbc_ih)();
int (keyboard_subscribe_int)(uint8_t *bit_no);
int (keyboard_unsubscribe_int)();
int (keyboard_enable_int)();
