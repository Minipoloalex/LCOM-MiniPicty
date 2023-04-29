#pragma once
#include <lcom/lcf.h>

#define RTC_ADDR_REG 0x70 /* Write here the address of the register to access */
#define RTC_DATA_REG 0x71 /* Write/Read the register specified */


#define RTC_SECONDS         0
#define RTC_SECONDS_ALARM   1
#define RTC_MINUTE          2
#define RTC_MINUTE_ALARM    3
#define RTC_HOUR            4
#define RTC_HOUR_ALARM      5
#define RTC_DAY_WEEK        6
#define RTC_DAY_MONTH       7
#define RTC_MONTH           8
#define RTC_YEAR            9

#define RTC_A 10
#define RTC_B 11
#define RTC_C 12  // write here to enable interrupts
#define RTC_D 13

#define RTC_A_UIP BIT(7)
// not relevant
// #define RTC_A_DV2 BIT(6)
// #define RTC_A_DV1 BIT(5)
// #define RTC_A_DV0 BIT(4)
// for periodic interrupts and square wave output
#define RTC_A_RS3 BIT(3)
#define RTC_A_RS2 BIT(2)
#define RTC_A_RS1 BIT(1)
#define RTC_A_RS0 BIT(0)

#define RTC_B_SET BIT(7)
#define RTC_B_PIE BIT(6)
#define RTC_B_AIE BIT(5)
#define RTC_B_UIE BIT(4)
#define RTC_B_SQWE BIT(3)
// shouldn't change this
// #define RTC_B_DM BIT(2)
// #define RTC_B_24_12 BIT(1)
#define RTC_B_DSE BIT(0)

#define RTC_C_IRQF BIT(7)
#define RTC_C_PF BIT(6) /* periodic interrupt pending */
#define RTC_C_AF BIT(5) /* alarm interrupt pending */
#define RTC_C_UF BIT(4) /* update interrupt pending */

#define RTC_D_VRT BIT(7)


int (rtc_read)(uint8_t reg, uint8_t *data);
int (rtc_write)(uint8_t reg, uint8_t data);

int (rtc_subscribe_int)(uint8_t *bit_no);
int (rtc_unsubscribe_int)();
int (rtc_ih)();

int (rtc_get_date)(uint8_t *day, uint8_t *month, uint8_t *year);
int (rtc_get_time)(uint8_t *hours, uint8_t *minutes, uint8_t *seconds);
int (rtc_get_date_time)(uint8_t *day, uint8_t *month, uint8_t *year, uint8_t *hours, uint8_t *minutes, uint8_t *seconds);

