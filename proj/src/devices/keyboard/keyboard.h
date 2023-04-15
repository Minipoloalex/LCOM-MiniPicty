#pragma once
#include <stdint.h>
#include <lcom/lcf.h>
#include "../controllers/i8042.h"
#include "../controllers/kbc/kbc.h"

int (keyboard_subscribe_interrupts)(uint8_t *bit_no);
int (keyboard_unsubscribe_interrupts)();
void (keyboard_ih)();
int (keyboard_restore)();
