#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

static int hook_id = TIMER0_IRQ;

int (convert_to_bcd)(uint16_t *ptr_value){
  uint16_t value = *ptr_value;
  if (value >= 10000) return EXIT_FAILURE;

  uint16_t digit;
  for(int i = 0; value;i++){
    digit = value % 10;
    value = value/10;
    *ptr_value |= digit<<(i*4);
  }

  return EXIT_SUCCESS;
}

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  uint8_t config;

  if(timer_get_conf(timer,&config) != 0) return EXIT_FAILURE;

  config &= TIMER_CLEAR_TIMER_SEL;
  config |= TIMER_LSB_MSB;

  switch (timer)
  {
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
  if (sys_outb(TIMER_CTRL,config) != 0) return EXIT_FAILURE;

  uint16_t value = (uint16_t) (TIMER_FREQ/freq);
  if(config & TIMER_BCD){
    if (convert_to_bcd(&value) != 0) return EXIT_FAILURE;
  }

  uint8_t lsb, msb;
  if(util_get_LSB(value, &lsb) != 0) return EXIT_FAILURE;
  if(util_get_MSB(value, &msb) != 0) return EXIT_FAILURE;

  if (sys_outb(TIMER(timer),lsb) != 0) return EXIT_FAILURE;
  if (sys_outb(TIMER(timer),msb) != 0) return EXIT_FAILURE;

  return EXIT_SUCCESS;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  *bit_no = hook_id;

  return sys_irqsetpolicy(TIMER0_IRQ,IRQ_REENABLE, &hook_id);
}

int (timer_unsubscribe_int)() {
  return sys_irqrmpolicy(&hook_id);
}

int counter = 0;
void (timer_int_handler)() {
  counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  uint8_t ctrl = TIMER_RB_CMD | TIMER_RB_SEL(timer) | TIMER_RB_COUNT_;

  if (sys_outb(TIMER_CTRL, ctrl) != 0) return EXIT_FAILURE;
  if (util_sys_inb(TIMER(timer), st) != 0) return EXIT_FAILURE;

  return EXIT_SUCCESS;
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
    union timer_status_field_val timer_status_field;
    uint8_t mask;

    switch(field){
        case tsf_all:
            timer_status_field.byte = st;
            break;
        case tsf_base:
            mask = BIT(0);
            timer_status_field.bcd = st & mask;
            break;
        case tsf_mode:
            mask = BIT(1) | BIT(2) | BIT(3);
            timer_status_field.count_mode = (st&mask)>>1;
            if ((timer_status_field.count_mode) > TIMER_NUMBER_MODES - 1){
                timer_status_field.count_mode &= 3;
            }
            break;
        case tsf_initial:
            mask = BIT(4) | BIT(5);
            timer_status_field.in_mode = (st&mask) >> 4;
            break;
    }
    
    if (timer_print_config(timer,field,timer_status_field)!= 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
