#include "keyboard.h"

static int hook_id;

int (keyboard_subscribe_interrupts)(uint8_t *bit_no){
  *bit_no = hook_id;
  return sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id);
}

int (keyboard_unsubscribe_interrupts)(){
  return sys_irqrmpolicy(&hook_id);
}

extern int return_value;
extern uint8_t scancode;

void kbc_ih(){
  if(read_KBC_output(KBC_OUT_REG, &scancode)){
    return_value = 1;
  } 
  else{
    return_value = 0;
  }
}

