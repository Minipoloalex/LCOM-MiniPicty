#pragma once
#include <lcom/lcf.h>

#define TIME_STRING_SIZE 8


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


/*
int (rtc_test_int)() { 
	rtc_init();
	uint8_t rtc_bit_no;
	rtc_subscribe_int(&rtc_bit_no);
	int ipc_status, r;
	message msg;
	uint32_t counter = 20;
	while (counter > 0) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d\n", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
				case HARDWARE:
					if (msg.m_notify.interrupts & BIT(rtc_bit_no)) {
						rtc_ih();
						counter--;
						char* a = rtc_get_current_time();
						printf("%s\n", a);
					}
					break;
				default:
					break;
			}
		}
		else {
		}
	}
	rtc_unsubscribe_int();
	return EXIT_SUCCESS;
}
*/
