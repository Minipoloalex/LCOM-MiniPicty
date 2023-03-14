#pragma once

#include <minix/sysutil.h>
#include "i8042.h"
#include <lcom/lcf.h>

int (read_KBC_status)(uint8_t *status);
int (read_KBC_output)(int8_t port, uint8_t* output, uint8_t  mouse);
int (write_KBC_command)(uint8_t port, uint8_t cmd_byte);
int (kbc_ih)(uint8_t mouse);
