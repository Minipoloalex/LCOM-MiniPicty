#pragma once

#include <lcom/lcf.h>
#include <stdint.h>
#include "uart.h"


int (set_base_addr)(uint16_t addr);

int (ser_read_line_control)(uint8_t *lcr);
int (ser_write_line_control)(uint8_t lcr);

/**
 * @brief Reads the divisor latch register
 * if lcr is NULL, it will read the line control register and set the DLAB bit to be able to access DLL and DLM
 * 
 */
int (ser_read_divisor)(uint16_t *divisor, uint8_t *lcr);
int (ser_write_divisor)(uint16_t divisor, uint8_t *lcr);

int (ser_read_int_enable)(uint8_t *ier, uint8_t *lcr);
int (ser_write_int_enable)(uint8_t ier, uint8_t *lcr);



int (ser_subscribe_int)(uint8_t *bit_no);
int (ser_unsubscribe_int)();
int (ser_enable_fifo)();

int (ser_set_baud_rate)(uint16_t rate);

int (ser_set_line_config)(uint8_t word_length, uint8_t stop_bit, uint8_t parity);

int (ser_enable_int)();
int (ser_disable_int)();
int (ser_write_char)(uint8_t c);
int (ser_read_char)(uint8_t *c);
int (ser_read_status)(uint8_t *status);
int (ser_read_int_id)(uint8_t *id);
int (ser_read_fifo)(uint8_t *fifo);
int (ser_write_fifo)(uint8_t fifo);
int (ser_read_modem)(uint8_t *modem);
int (ser_write_modem)(uint8_t modem);
int (ser_read_line)(uint8_t *line);
int (ser_write_line)(uint8_t line);


int (ser_read_data)(uint8_t *data);
int (ser_write_data)(uint8_t data);

int (ser_read_int_id_fifo)(uint8_t *int_id_fifo);
int (ser_write_int_id_fifo)(uint8_t int_id_fifo);


int (ser_read_modem_control)(uint8_t *modem_control);
int (ser_write_modem_control)(uint8_t modem_control);
int (ser_read_line_status)(uint8_t *line_status);
int (ser_write_line_status)(uint8_t line_status);
int (ser_read_modem_status)(uint8_t *modem_status);
int (ser_write_modem_status)(uint8_t modem_status);
int (ser_read_scratch)(uint8_t *scratch);
int (ser_write_scratch)(uint8_t scratch);
