#include "rtc.h"

#define RTC_IRQ 8

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
#define RTC_C 12
#define RTC_D 13

#define RTC_A_UIP BIT(7)

#define RTC_B_SET BIT(7)
#define RTC_B_PIE BIT(6)
#define RTC_B_AIE BIT(5)
#define RTC_B_UIE BIT(4)

#define RTC_B_DM BIT(2)
#define RTC_B_DSE BIT(0)

#define RTC_C_IRQF BIT(7)
#define RTC_C_PF BIT(6) /** @brief periodic interrupt pending */
#define RTC_C_AF BIT(5) /** @brief alarm interrupt pending */
#define RTC_C_UF BIT(4) /** @brief update interrupt pending */

#define MAX_TIME_DIFFERENCE 1
#define INITIAL_HOUR_VALUE 255

static char *currentTime = {"hh mm ss"};

static int hook_id = RTC_IRQ;

int rtc_return_value = EXIT_SUCCESS;

static uint8_t hours = INITIAL_HOUR_VALUE, minutes = 0, seconds = 0;
static bool stable = false;
static bool isBCD = false;
/**
 * @brief Reads from the RTC given register.
 * @param reg register to read from. Can be all the registers from RTC_SECONDS to RTC_D
 * @param data pointer to the variable where the data will be stored
 */
int (rtc_read)(uint8_t reg, uint8_t *data);
/**
 * @brief Writes to the RTC given register.
 * @param reg register to write to. Can be all the registers from RTC_SECONDS to RTC_D
 * @param data pointer to the data to write to the register
 * @return 0 upon success, non-zero otherwise
 */
int (rtc_write)(uint8_t reg, uint8_t data);
/**
 * @brief Gets the time difference between the current time and the new time. Can be used to test if the RTC has been "stable" for a certain amount of time (tests consistency of the read values). Should not be called if attributes hours, minutes and seconds have not been initialized yet.
 * @param new_hours new hours value
 * @param new_minutes new minutes value
 * @param new_seconds new seconds value
 * @return the absolute value of the difference between the current time and the new time (in seconds).
 */
int32_t (time_difference)(uint8_t new_hours, uint8_t new_minutes, uint8_t new_seconds);
/**
 * @brief Increments the time by one second. Increments the variables hours, minutes and seconds.
 * Should be called from `rtc_ih()` in case the RTC is "stable".
 */
void (increment_one_second)();
/**
 * @brief Gets the current time from the RTC registers. Should be called from `rtc_ih()`.
 * @param _hours pointer to the variable where the hours will be stored
 * @param _minutes pointer to the variable where the minutes will be stored
 * @param _seconds pointer to the variable where the seconds will be stored
 */
int (rtc_get_time)(uint8_t *_hours, uint8_t *_minutes, uint8_t *_seconds);
/**
 * @brief Converts the given value from BCD to binary. Assumes the value is in BCD.
 * @param ptr pointer to the value to convert. Also stores the converted value.
 */
int (convert_bcd_to_binary)(uint8_t *ptr);

/*===============================================================================================*/

int (rtc_init)() {
  uint8_t regB;
  if (rtc_read(RTC_B, &regB)) {
    printf("Error reading RTC_B\n");
    return EXIT_FAILURE;
  }
  regB |= RTC_B_UIE;
  regB &= ~(RTC_B_PIE | RTC_B_AIE);
  isBCD = !(regB & RTC_B_DM);
  if (rtc_write(RTC_B, regB)) {
    printf("Error writing RTC_B\n");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int (rtc_subscribe_int)(uint8_t *bit_no) {
  uint8_t regC;
  rtc_read(RTC_C, &regC);
  *bit_no = hook_id;
  if (sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id)) {
    printf("Error subscribing RTC\n");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int (rtc_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_id)) {
    printf("Error unsubscribing RTC\n");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

void (rtc_ih)() {
  uint8_t regC;
  if(rtc_read(RTC_C, &regC)) {
    printf("Error reading RTC_C\n");
    rtc_return_value = EXIT_FAILURE;
    return;
  }
  if (!(regC & RTC_C_IRQF)) {
    printf("no interrupt pending inside rtc_ih\n");
    rtc_return_value = EXIT_FAILURE;
    return;
  }
  if (regC & RTC_C_UF) {
    if (stable) {
      increment_one_second();
    }
    else {
      uint8_t new_hours, new_minutes, new_seconds;
      if (rtc_get_time(&new_hours, &new_minutes, &new_seconds)) {
        printf("Error getting time\n");
        rtc_return_value = EXIT_FAILURE;
        return;
      }

      if (hours != INITIAL_HOUR_VALUE) {  // if the hours have been initialized
        int32_t time_diff = time_difference(new_hours, new_minutes, new_seconds);
        if (time_diff == MAX_TIME_DIFFERENCE) {
          stable = true;
        }
      }
      hours = new_hours;
      minutes = new_minutes;
      seconds = new_seconds;
    }
    rtc_return_value = EXIT_SUCCESS;
    return;
  }
  printf("Error: RTC_C_UF not set - we are only using this interrupt for RTC\n");
  rtc_return_value = EXIT_FAILURE;
}

int (rtc_read)(uint8_t reg, uint8_t *data) {
  if (reg > RTC_D) {
    printf("Invalid register\n");
    return EXIT_FAILURE;
  }
  if(sys_outb(RTC_ADDR_REG, reg)) {
    printf("Error writing to RTC_ADDR_REG\n");
    return EXIT_FAILURE;
  }
  if(util_sys_inb(RTC_DATA_REG, data)) {
    printf("Error reading from RTC_DATA_REG\n");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int (rtc_write)(uint8_t reg, uint8_t data) {
  if (reg > RTC_D) {
    printf("Invalid register\n");
    return EXIT_FAILURE;
  }
  if(sys_outb(RTC_ADDR_REG, reg)) {
    printf("Error writing to RTC_ADDR_REG\n");
    return EXIT_FAILURE;
  }
  if(sys_outb(RTC_DATA_REG, data)) {
    printf("Error writing to RTC_DATA_REG\n");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

void (increment_one_second)() {
  seconds++;
  if (seconds == 60) {
    seconds = 0;
    minutes++;
    if (minutes == 60) {
      minutes = 0;
      hours++;
      if (hours == 24) {
        hours = 0;
      }
    }
  }
}
/**
 * @brief Used to check whether the RTC value read is valid or not
 * @return the absolute value of the difference between the current time and the new time (in seconds).
 */
int32_t (time_difference)(uint8_t new_hours, uint8_t new_minutes, uint8_t new_seconds) {
  uint32_t current = hours * 3600 + minutes * 60 + seconds;
  uint32_t new = new_hours * 3600 + new_minutes * 60 + new_seconds;
  return new - current;
}

int (rtc_get_time)(uint8_t *_hours, uint8_t *_minutes, uint8_t *_seconds) {
  if (_hours == NULL || _minutes == NULL || _seconds == NULL) {
    printf("Invalid pointers inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if (rtc_read(RTC_HOUR, _hours)) return EXIT_FAILURE;
  if (rtc_read(RTC_MINUTE, _minutes)) return EXIT_FAILURE;
  if (rtc_read(RTC_SECONDS, _seconds)) return EXIT_FAILURE;
  if (isBCD) {
    convert_bcd_to_binary(_hours);
    convert_bcd_to_binary(_minutes);
    convert_bcd_to_binary(_seconds);
  }
  return EXIT_SUCCESS;
}

char* (rtc_get_current_time)() {
  if (hours == INITIAL_HOUR_VALUE) {
    return NULL;
  }
  currentTime[0] = hours / 10 + '0';
  currentTime[1] = hours % 10 + '0';
  currentTime[3] = minutes / 10 + '0';
  currentTime[4] = minutes % 10 + '0';
  currentTime[6] = seconds / 10 + '0';
  currentTime[7] = seconds % 10 + '0';
  return currentTime;
}

uint8_t (rtc_get_hour)() {
  return hours;
}
int (rtc_read_temp_hour)(uint8_t *hour) {
  if (rtc_read(RTC_HOUR, hour)) {
    printf("Error reading RTC_HOUR\n");
    return EXIT_FAILURE;
  }
  if (isBCD) {
    convert_bcd_to_binary(hour);
  }
  return EXIT_SUCCESS;
}

int (convert_bcd_to_binary)(uint8_t *ptr) {
  if (ptr == NULL) {
    printf("Invalid pointer inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  *ptr = (*ptr & 0x0F) + ((*ptr & 0xF0) >> 4) * 10;
  return EXIT_SUCCESS;
}
