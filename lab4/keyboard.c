#include "keyboard.h"

static int hook_id = IRQ_KEYBOARD;
int return_value_keyboard = 0;
uint8_t scancode_byte = 0;

int (keyboard_subscribe_interrupts)(uint8_t *bit_no){
  *bit_no = hook_id;
  return sys_irqsetpolicy(IRQ_KEYBOARD, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id);
}

int (keyboard_unsubscribe_interrupts)(){
  return sys_irqrmpolicy(&hook_id);
}

void (keyboard_ih)(){
  return_value_keyboard = read_KBC_output(KBC_OUT_REG, &scancode_byte, false);
}

int (keyboard_enable_int)(){
  uint8_t commandWord;
  if(write_KBC_command(KBC_IN_REG, KBC_READ_CMD_BYTE) != 0) return EXIT_FAILURE;
  if(read_KBC_output(KBC_OUT_REG, &commandWord, 0) != 0) return EXIT_FAILURE;

  commandWord |= KEYBOARD_ENABLE_INT;
  
  if(write_KBC_command(KBC_IN_REG, KBC_WRITE_CMD_BYTE) != 0) return EXIT_FAILURE;
  return write_KBC_command(KBC_OUT_REG, commandWord);
}
