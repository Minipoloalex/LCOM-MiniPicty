#pragma once

#include <lcom/lcf.h>
#include <stdint.h>
#include "uart.h"
#include "queue.h"

int (ser_init)(uint16_t base_addr, uint32_t baud_rate, uint8_t word_length, uint8_t stop_bit, int8_t parity);
int (ser_subscribe_int)(uint8_t *bit_no);
int (ser_unsubscribe_int)();

int (ser_write_char)(uint8_t c);
int (ser_read_char)(uint8_t *c);

int (ser_read_char_int)(uint8_t *data);
int (ser_write_char_int)(uint8_t data);


int (ser_read_int_id)(uint8_t *id);


void (ser_ih_fifo)();

int (ser_write_fifo_control_default)();

int (ser_add_byte_to_transmitter_queue)(uint8_t c);
int (ser_read_bytes_from_receiver_queue)();


