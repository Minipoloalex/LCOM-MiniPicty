#include "timer.h"

static int hook_id = TIMER0_IRQ;

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
  if (timer > 2 || freq == 0) return EXIT_FAILURE;
  uint8_t config;
  if (timer_get_conf(timer, &config) != 0) return EXIT_FAILURE;
  config &= TIMER_CLEAR_TIMER_SEL;
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
  *bit_no = hook_id;
  return sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id);
}

int (timer_unsubscribe_int)() {
  return sys_irqrmpolicy(&hook_id);
}

int timer_counter = 0;
void (timer_int_handler)() {
  timer_counter++;

  // TODO: Change the draw lines and etc to here!!
  // do not put inside timer_int_handler anything else: make another function and call it from somewhere else
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if (timer > 2) return EXIT_FAILURE;
  if (sys_outb(TIMER_CTRL, TIMER_RB_CMD | TIMER_RB_SEL(timer) | TIMER_RB_COUNT_) != 0) 
    return EXIT_FAILURE;
    
  if (util_sys_inb(TIMER(timer), st) != 0) return EXIT_FAILURE;

  return EXIT_SUCCESS;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  union timer_status_field_val timer_status_field;
  if (timer > 2) return EXIT_FAILURE;
  switch (field) {
    case tsf_all:
      timer_status_field.byte = st;
      break;
    case tsf_base:
      timer_status_field.bcd = st & TIMER_BASE_MASK;
      break;
    case tsf_mode:
      timer_status_field.count_mode = (st & TIMER_MODE_MASK) >> TIMER_COUNT_MODE_POSITION;
      if ((timer_status_field.count_mode) > TIMER_NUMBER_MODES - 1) {
        timer_status_field.count_mode &= 3; /* e.g. mode 110 is mode 2 */
      }
      break;
    case tsf_initial:
      switch((st & TIMER_INITIAL_MASK ) >> TIMER_INITIAL_MODE_POSITION){
        case(0):
          timer_status_field.in_mode = INVAL_val;
          break;
        case(1):
          timer_status_field.in_mode = LSB_only;
          break;
        case(2):
          timer_status_field.in_mode = MSB_only;
          break;
        case(3):
          timer_status_field.in_mode = MSB_after_LSB;
          break;
      }
      break;
  }
  return timer_print_config(timer, field, timer_status_field);
}
