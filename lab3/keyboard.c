#include "keyboard.h"

static int hook_id = IRQ_KEYBOARD;

int (keyboard_subscribe_interrupts)(uint8_t *bit_no){
  *bit_no = hook_id;
  return sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id);
}

int (keyboard_unsubscribe_interrupts)(){
  return sys_irqrmpolicy(&hook_id);
}

int return_value = 0;
uint8_t scancode = 0;

void (kbc_ih)(){
  if(read_KBC_output(KBC_OUT_REG, &scancode)){
    return_value = 1;
  }
  else{
    return_value = 0;
  }
}

int (keyboard_restore)(){
  uint8_t commandWord;
  if(write_KBC_command(KBC_CMD_REG, KBC_READ_CMD) != 0) return EXIT_FAILURE;
  if(read_KBC_output(KBC_OUT_REG, &commandWord) != 0) return EXIT_FAILURE;

  commandWord |= ENABLE_INT;
  
  if(write_KBC_command(KBC_CMD_REG, KBC_WRITE_CMD) != 0) return EXIT_FAILURE;
  return write_KBC_command(KBC_OUT_REG, commandWord);
}
