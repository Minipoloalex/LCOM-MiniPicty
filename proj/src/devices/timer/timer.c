#include "timer.h"

static int hook_id = TIMER0_IRQ;

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
}
