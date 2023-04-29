#include "rtc.h"

#define RTC_IRQ 8

static int hook_id = RTC_IRQ;

int (rtc_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) {
    printf("Invalid pointer\n");
    return EXIT_FAILURE;
  }
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

int (rtc_ih)() {
  uint8_t regC;
  if(rtc_read(RTC_C, &regC)) {
    printf("Error reading RTC_C\n");
    return EXIT_FAILURE;
  }
  if (regC & RTC_C_UF) {
    printf("Update Complete\n");
  }
  return EXIT_SUCCESS;
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

int (rtc_get_date)(uint8_t *day, uint8_t *month, uint8_t *year) {
  if(rtc_read(RTC_DAY_MONTH, day)) {
    printf("Error reading day\n");
    return EXIT_FAILURE;
  }
  if(rtc_read(RTC_MONTH, month)) {
    printf("Error reading month\n");
    return EXIT_FAILURE;
  }
  if(rtc_read(RTC_YEAR, year)) {
    printf("Error reading year\n");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
