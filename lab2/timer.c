#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

<<<<<<< HEAD
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
=======
static int HOOK_ID;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  uint8_t config;
  if(timer_get_conf(timer, &config)){
    printf("Error on timer_get_conf() called by timer_set_frequency()");
    return 1;
  }

  uint8_t TIMER_SEL;
  switch (timer){
    case 0:
      TIMER_SEL = TIMER_SEL0;
      break;
    case 1:
      TIMER_SEL = TIMER_SEL1;
      break;
    case 2:
      TIMER_SEL = TIMER_SEL2;
      break;
  }

  config &= 0x0F;  // config = 0b 0000[MODE][BCD?]

  // Command word to write a new counting value on the timer
  uint8_t cmd_word = TIMER_SEL | TIMER_LSB_MSB;  // cmd_word = 0b [TIMER_SEL][11]0000
  cmd_word |= config;  // cmd_word = [TIMER_SEL][11][MODE][BCD?]

  if(sys_outb(TIMER_CTRL, cmd_word)){
    printf("Error sys_outb(): writing cmd_word on TIMER_CNTRL");
    return 1;
  }

  // Calculate the timer's new value
  uint16_t value = TIMER_FREQ / freq;
  uint8_t lsb;
  util_get_LSB(value, &lsb);
  uint8_t msb;
  util_get_MSB(value, &msb);

  // Write the new value on the timer
  if(sys_outb(TIMER_0 + timer, lsb)){
    printf("Error on sys_outb(): writing lsb on timer");
    return 1;
  }
  if(sys_outb(TIMER_0 + timer, msb)){
    printf("Error on sys_outb(): writing msb on timer");
    return 1;
  }
  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {   // TODO: what is bit_no?
  *bit_no = HOOK_ID;
  return sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, bit_no);
}

int (timer_unsubscribe_int)() {
  return sys_irqrmpolicy(&HOOK_ID);
>>>>>>> francisco_ana
}

int counter = 0;
void (timer_int_handler)() {
  counter++;
}

<<<<<<< HEAD
int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if (sys_outb(TIMER_CTRL, TIMER_RB_CMD | TIMER_RB_SEL(timer) | TIMER_RB_COUNT_) != 0) 
    return EXIT_FAILURE;
    
  if (util_sys_inb(TIMER(timer), st) != 0) return EXIT_FAILURE;

  return EXIT_SUCCESS;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  union timer_status_field_val timer_status_field;
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
  if (timer_print_config(timer, field, timer_status_field) != 0) return EXIT_FAILURE;
  return EXIT_SUCCESS;
=======
int (timer_get_conf)(uint8_t timer, uint8_t* st) {
  if(sys_outb(TIMER_CTRL, TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer))){
    printf("Error on sys_outb() called by timer_get_conf()");
    return 1;
  }
  if(util_sys_inb(TIMER_0 + timer, st)){
    printf("Error on util_sys_inb() called by timer_get_conf()");
    return 1;
  }
  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  union timer_status_field_val val;

  uint8_t mask;
  switch(field){
    case tsf_all:
      val.byte = st;
      break;
    case tsf_initial:
      mask = 0x30;
      switch ((st & mask) >> 4){
        case 1:
          val.in_mode = LSB_only;
          break;
        case 2:
          val.in_mode = MSB_only;
          break;
        case 3:
          val.in_mode = MSB_after_LSB;
          break;
      }
      break;
    case tsf_mode:
      mask = 0x0E;
      switch((st & mask) >> 1){
        case 0:
          val.count_mode = 0;
          break;
        case 1:
          val.count_mode = 1;
          break;
        case 2:
        case 6:
          val.count_mode = 2;
          break;
        case 3:
        case 7:
          val.count_mode = 3;
          break;
        case 4:
          val.count_mode = 4;
          break;
        case 5:
          val.count_mode = 5;
          break;
      }
      break;
    case tsf_base:
      val.bcd = st & 1;
      break;
  }
  return timer_print_config(timer, field, val);
>>>>>>> francisco_ana
}
