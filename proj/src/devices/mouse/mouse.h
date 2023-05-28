#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <lcom/lcf.h>

#include "../controllers/i8042.h"
#include "../controllers/kbc/kbc.h"
#include "../video_card/video_card.h"
#include "../../model/position/position.h"

/** @defgroup mouse Mouse
 * @{
 *
 * @brief Module responsible for the mouse
 */

/**
 * @brief returns true if the packet is ready to be processed and false otherwise
 * 
 */
bool (packet_is_ready)();
/**
 * @brief returns the packet bytes
 * 
 */
uint8_t *(mouse_get_packet_bytes)();
/**
 * @brief Inserts packets data into the packet struct
 * @returns 0 upon success, non-zero otherwise
 */
int (mouse_process_packet_byte)();
/**
 * @brief Mouse interrupt handler
 * Reads mouse data and checks if it is valid
 */
void (mouse_ih)();
/**
 * @brief returns a updated drawing position based on a new mouse packet and the previous position
 * Always call this if and only if packet_is_ready() returns true
 */
drawing_position_t (mouse_get_drawing_position_from_packet)(position_t before_position);

/**
 * @brief Subscribes and enables Mouse interrupts
 * 
 * @param bit_no address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
 * @return 0 upon success, non-zero otherwise
 */
int (mouse_subscribe_interrupts)(uint8_t *bit_no);
/**
 * @brief Unsubscribes Mouse interrupts
 * @return 0 upon success, non-zero otherwise
 */
int (mouse_unsubscribe_interrupts)();
/**
 * @brief Enables mouse interrupts
 * @return 0 upon success, non-zero otherwise 
 */
int (mouse_enable_int)();
/**
 * @brief Disables mouse interrupts
 * @return 0 upon success, non-zero otherwise 
 */
int (mouse_disable_int)();
/**
 * @brief Enables mouse data reporting mode
 * @return 0 upon success, non-zero otherwise 
 */
int (mouse_enable_data_report)();
/**
 * @brief Disables mouse data reporting mode
 * @return 0 upon success, non-zero otherwise
 */
int (mouse_disable_data_report)();
