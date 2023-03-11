#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "i8042.h"
#include "kbc.h"

extern int return_value;
extern uint8_t scancode;

int (keyboard_subscribe_interrupts)(uint8_t* bit_no);
int (keyboard_unsubscribe_interrupts)();
void (kbc_ih)();
int (keyboard_restore)();

#endif