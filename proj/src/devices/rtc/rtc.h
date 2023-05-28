#pragma once
#include <lcom/lcf.h>

#define TIME_STRING_SIZE 8

/** @defgroup rtc RTC
 * @{
 *
 * @brief Module responsible for the real-time clock
 */

/**
 * @brief initializes the RTC. Should be called before any other function.
 * @return 0 upon success, non-zero otherwise
 */
int (rtc_init)();
/**
 * @brief Subscribes RTC interrupts.
 * @param bit_no pointer to the variable that will store the bit mask returned upon an interrupt
 * @return 0 upon success, non-zero otherwise
 */
int (rtc_subscribe_int)(uint8_t *bit_no);
/**
 * @brief Unsubscribes RTC interrupts. `rtc_ih()` should not be called after this.
 * @return 0 upon success, non-zero otherwise
 */
int (rtc_unsubscribe_int)();
/**
 * @brief Interrupt handler for RTC interrupts. Should be called upon an RTC interrupt.
 * To get the time, use `rtc_get_current_time()`, after this.
 */
void (rtc_ih)();
/**
 * @brief Returns the current time string in the format hh:mm:ss, size given by TIME_STRING_SIZE.
 * @return pointer to the current time string. This string should not be freed. Returns null in case the time has not been initialized yet, so it should be tested if the value is NULL before using it.
 */
char* (rtc_get_current_time)();
/**
 * @brief Returns the current hour.
 * 
 */
uint8_t (rtc_get_hour)();
/**
 * @brief Writes the current hour to the given pointer.
 * 
 * @return return 0 upon success, non-zero otherwise
 */
int (rtc_read_temp_hour)(uint8_t *hour);
