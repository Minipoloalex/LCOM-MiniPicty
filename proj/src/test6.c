#include "test6.h"

// int (rtc_test_conf)(void) {
// 	uint8_t regA, regB, regC, regD;
// 	if(rtc_read(RTC_A, &regA)) return 1;
// 	if(rtc_read(RTC_B, &regB)) return 1;
// 	if(rtc_read(RTC_C, &regC)) return 1;
// 	if(rtc_read(RTC_D, &regD)) return 1;
// 	printf("regA: %02x\n", regA);
// 	printf("regB: %02x\n", regB);
// 	printf("regC: %02x\n", regC);
// 	printf("regD: %02x\n", regD);
// 	return EXIT_SUCCESS;
// }

// int (rtc_test_date)(void) {
// 	uint8_t day, month, year;
// 	if(rtc_get_date(&day, &month, &year)) {
// 		printf("Error getting date\n");
// 		return EXIT_FAILURE;
// 	}
// 	printf("day: %02x\n", day);
// 	printf("month: %02x\n", month);
// 	printf("year: %02x\n", year);
// 	return EXIT_SUCCESS;
// }

int (rtc_test_int)(/* to be defined in class */) { 
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
