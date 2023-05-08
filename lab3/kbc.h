#pragma once
#include <lcom/lcf.h>

#include <minix/sysutil.h>
#include <stdint.h>

#include "i8042.h"

int (read_kbc_status)(uint8_t *status);
int (read_kbc_output)(int port, uint8_t *output);
int (write_kbc_command)(int port, uint8_t command);
