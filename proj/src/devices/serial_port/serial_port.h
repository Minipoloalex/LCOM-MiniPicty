#pragma once

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdbool.h>
#include "../controllers/uart.h"
#include "../../model/queue/queue.h"
#include "../../modules/game/player_drawer/player_drawer.h"
#include "../../model/position/position.h"
#include "../../model/button/button.h"
#include "../../model/state/state.h"
/** @defgroup serial_port SerialPort
 * @{
 *
 * @brief Module responsible for the serial port
 */
/**
 * @brief Sets the default communication parameters and enables the serial port interrupts. Also creates a transmitter and a receiver queue. These parameters are: 8 bits per character, 1 stop bit, odd parity and 115200 bits per second (max bitrate).
 */
int (ser_init)();
/**
 * @brief Destroys the queues created by ser_init()
 */
void (delete_ser)();
/**
 * @brief Subscribes the serial port interrupts
 * @param bit_no Address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
 */
int (ser_subscribe_int)(uint8_t *bit_no);
/**
 * @brief Unsubscribes the serial port interrupts
 */
int (ser_unsubscribe_int)();
/**
 * @brief Interrupt handler for the serial port. Checks which interrupt was triggered and either sends bytes to the fifo from the transmitter queue, or reads them from the fifo into the receiver queue.
 */
void (ser_ih_fifo)();
/**
 * @brief Reads bytes from the serial port fifo into the receiver queue
 */
int (ser_read_from_fifo)();
/**
 * @brief Writes bytes from the transmitter queue into the serial port fifo
 */
int (ser_write_to_fifo)();
/**
 * @brief Adds given position to the transmitter queue according to the program's communication protocol.
 * @param position Position to be added to the transmitter queue
 */
int (ser_add_position_to_transmitter_queue)(drawing_position_t position);
/**
 * @brief Adds given button index to the transmitter queue according to the program's communication protocol.
 * @param index Index of the button to be added to the transmitter queue
 */
int (ser_add_button_click_to_transmitter_queue)(uint8_t index);
/**
 * @brief Adds given word index to the transmitter queue according to the program's communication protocol.
 * @param index Index of the word to be added to the transmitter queue
 */
int (ser_add_word_index)(uint8_t index);
/**
 * @brief Adds some bytes to the transmitter queue according to the program's communication protocol, indicating the round was won.
 */
int (ser_add_won_round)();
/**
 * @brief Reads bytes from the receiver queue according to the program's communication protocol, and updates the player's position, app_state and won_round accordingly.
 * It processes only bytes that come from ser_add_position_to_transmitter_queue, ser_add_button_click_to_transmitter_queue, ser_add_word_index and ser_add_won_round.
 * @param drawer Player drawer to be updated
 * @param app_state App state to be updated
 * @param won_round Won round to be updated
 */
int (ser_read_bytes_from_receiver_queue)(player_drawer_t *drawer, state_t *app_state, bool *won_round);
