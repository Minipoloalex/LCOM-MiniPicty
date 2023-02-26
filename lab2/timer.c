#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int (convert_to_bcd)(uint16_t *ptr_value) {
  uint16_t value = *ptr_value;
  if (value >= 10000) return EXIT_FAILURE; /* doesn't fit in BCD */
  
  uint16_t digit;
  *ptr_value = 0;

  for (int i = 0; value; i++) {
    digit = value % 10;
    value = value / 10;
    *ptr_value |= digit << (i * 4);
  }
  return EXIT_SUCCESS;
}

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  uint8_t config;
  if (timer_get_conf(timer, &config) != 0) return EXIT_FAILURE;
  config &= 0x3F;
  config |= TIMER_LSB_MSB;
  switch (timer) {
    case 0:{
      config |= TIMER_SEL0;
      break;
    }
    case 1:{
      config |= TIMER_SEL1;
      break;
    }
    case 2:{
      config |= TIMER_SEL2;
      break;
    }
  }
  if (sys_outb(TIMER_CTRL, config) != 0) return EXIT_FAILURE;

  uint16_t value = (uint16_t) (TIMER_FREQ / freq);

  if (config & TIMER_BCD) {
    if (convert_to_bcd(&value) != 0) return EXIT_FAILURE;
  }
  uint8_t lsb = 0;
  if (util_get_LSB(value, &lsb) != 0) return EXIT_FAILURE;
  uint8_t msb = 0;
  if (util_get_MSB(value, &msb) != 0) return EXIT_FAILURE;

  if (sys_outb(TIMER(timer), lsb) != 0) return EXIT_FAILURE;
  if (sys_outb(TIMER(timer), msb) != 0) return EXIT_FAILURE;

  return EXIT_SUCCESS;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
    /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int (timer_unsubscribe_int)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

void (timer_int_handler)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  uint8_t ctrl = TIMER_RB_CMD | TIMER_RB_SEL(timer) | TIMER_RB_COUNT_;
  /* selects timer and Read back command; bit 1 to not get count, bit 0 to get status */

  if (sys_outb(TIMER_CTRL, ctrl) != 0) return EXIT_FAILURE;
  if (util_sys_inb(TIMER(timer), st) != 0) return EXIT_FAILURE;

  return EXIT_SUCCESS;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  union timer_status_field_val timer_status_field;
  uint8_t mask;
  switch (field) {
    case tsf_all:
      timer_status_field.byte = st;
      break;
    case tsf_base:
      mask = BIT(0);
      timer_status_field.bcd = st & mask;
      break;
    case tsf_mode:
      mask = BIT(1) | BIT(2) | BIT(3);
      timer_status_field.count_mode = (st & mask) >> 1;
      if ((timer_status_field.count_mode) > TIMER_NUMBER_MODES - 1) {
        timer_status_field.count_mode &= 3; /* e.g. mode 110 is mode 2 */
      }
      break;
    case tsf_initial:
      mask = BIT(4) | BIT(5);
      timer_status_field.in_mode = (st & mask) >> 4;
    break;
  }
  if (timer_print_config(timer, field, timer_status_field) != 0) return EXIT_FAILURE;
  return EXIT_SUCCESS;
}
