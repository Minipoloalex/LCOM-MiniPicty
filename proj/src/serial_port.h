#pragma once

#include <lcom/lcf.h>
#include <stdint.h>
#include "uart.h"
#include "queue.h"

int (ser_set_base_addr)(uint16_t addr, uint8_t is_tr);

int (ser_read_line_control)(uint8_t *lcr);
int (ser_write_line_control)(uint8_t lcr);

int (ser_set_line_config)(uint8_t word_length, uint8_t stop_bit, int8_t parity);

/**
 * @brief Reads the divisor latch register
 * Reads the line control register and sets the DLAB bit to be able to access DLL and DLM (if it is not already set)
 * @param divisor pointer to the variable that will store the divisor
 * @param lcr pointer to the variable that may store the line control register (can be NULL and in that case it will be read and updated)
 * @return 0 if everything went well, different than 0 otherwise
 */
int (ser_read_divisor)(uint16_t *divisor);
int (ser_write_divisor)(uint16_t divisor);
int (ser_set_baud_rate)(uint32_t rate); // TODO: implement this


int (ser_read_int_enable)(uint8_t *ier);
int (ser_write_int_enable)(uint8_t ier);  // not implemented



int (ser_read_line_status)(uint8_t *status);

/**
 * @brief Reads data from the Receiver Buffer Register (RBR)
 * Assumes that the data is ready to be read
 */
int (ser_read_data)(uint8_t *data);
/**
 * @brief Writes data to the Transmitter Holding Register (THR)
 * Assumes that the data is ready to be written
 */
int (ser_write_data)(uint8_t data);

int (ser_write_char)(uint8_t c);
int (ser_read_char)(uint8_t *c);

int (ser_read_char_int)(uint8_t *data);
int (ser_write_char_int)(uint8_t data);


int (ser_read_int_id)(uint8_t *id);



int (ser_subscribe_int)(uint8_t *bit_no);
int (ser_unsubscribe_int)();

void (ser_ih)();
int (ser_add_byte_to_transmitter_queue)(uint8_t c);




int (ser_read_int_id_fifo)(uint8_t *int_id_fifo);
int (ser_write_int_id_fifo)(uint8_t int_id_fifo);

int (ser_write_fifo_control)(uint8_t config);
int (ser_write_fifo_control_default)();
int (ser_read_bytes_from_receiver_queue)();
void (ser_ih_fifo)();









int (ser_enable_fifo)();

int (ser_enable_int)();
int (ser_disable_int)();


int (ser_read_fifo)(uint8_t *fifo);
int (ser_write_fifo)(uint8_t fifo);
