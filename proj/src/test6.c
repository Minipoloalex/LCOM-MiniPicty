#include "test6.h"

int (rtc_test_conf)(void) {
	uint8_t regA, regB, regC, regD;
	if(rtc_read(RTC_A, &regA)) return 1;
	if(rtc_read(RTC_B, &regB)) return 1;
	if(rtc_read(RTC_C, &regC)) return 1;
	if(rtc_read(RTC_D, &regD)) return 1;
	printf("regA: %02x\n", regA);
	printf("regB: %02x\n", regB);
	printf("regC: %02x\n", regC);
	printf("regD: %02x\n", regD);
	return EXIT_SUCCESS;
}

int (rtc_test_date)(void) {
	uint8_t day, month, year;
	if(rtc_get_date(&day, &month, &year)) {
		printf("Error getting date\n");
		return EXIT_FAILURE;
	}
	printf("day: %02x\n", day);
	printf("month: %02x\n", month);
	printf("year: %02x\n", year);
	return EXIT_SUCCESS;
}

int (rtc_test_int)(/* to be defined in class */) { 
	return EXIT_SUCCESS;
}

