#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  uint8_t config;
  if (timer_get_conf(timer, &config) != 0) return 1;
  
  config &= 0x0F; // clear unneccessary bits
  config |= TIMER_LSB_MSB; // lsb and msb (by this order)

  switch (timer) {
    case 0:
      config |= TIMER_SEL0;
      break;
    case 1:
      config |= TIMER_SEL1;
      break;
    case 2:
      config |= TIMER_SEL2;
      break;
  }

  if (sys_outb(TIMER_CTRL, config)) return 1;

  uint16_t value = TIMER_FREQ / freq;  

  uint8_t lsb = 0;
  util_get_LSB(value, &lsb);
  if (sys_outb(TIMER_0 + timer, lsb)) return 1;

  uint8_t msb = 0;
  util_get_MSB(value, &msb);
  return sys_outb(TIMER_0 + timer, msb);
}

static int hook_id = TIMER0_IRQ;

int (timer_subscribe_int)(uint8_t *bit_no) {
  *bit_no = hook_id;
  return sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id);
}

int (timer_unsubscribe_int)() {
  return sys_irqrmpolicy(&hook_id);
}

int counter = 0;
void (timer_int_handler)() {
  counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if (sys_outb(TIMER_CTRL, TIMER_RB_CMD | TIMER_RB_SEL(timer) | TIMER_RB_COUNT_) !=0) return 1;
  return util_sys_inb(TIMER_0 + timer, st);
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  union timer_status_field_val tsfv;
  switch (field) {
    case tsf_all:
      tsfv.byte = st;
      break;
    case tsf_base:
      tsfv.bcd = st & BIT(0); // TIMER_BASE_MASK BIT(0)
      break;
    case tsf_initial:
    // TIMER_INITIAL_MASK (BIT(4) | BIT(5))
      switch ((st & (BIT(4) | BIT(5))) >> 4) {
        case 0:
          tsfv.in_mode = INVAL_val;
          break;
        case 1:
          tsfv.in_mode = LSB_only;
          break;
        case 2:
          tsfv.in_mode = MSB_only;
          break;
        case 3:
          tsfv.in_mode = MSB_after_LSB;
          break;
      }
      break;
    case tsf_mode:
    // TIMER_MODE_MASK BIT(1) | BIT(2) | BIT(3)
      tsfv.count_mode = (st & (BIT(1) | BIT(2) | BIT(3))) >> 1;
      if (tsfv.count_mode > 5) {
        tsfv.count_mode &= 0x03;
      }
      break;
  }
  return timer_print_config(timer, field, tsfv);
}
