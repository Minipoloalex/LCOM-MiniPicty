#pragma once
#include <stdint.h>

int (keyboard_subscribe_int)(uint8_t *bit_no);

int (keyboard_unsubscribe_int)();

void (kbc_ih)();

/* TODO: create documentation here */
void (enable_int)();
