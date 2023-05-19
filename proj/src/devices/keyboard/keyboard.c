#include "keyboard.h"

static int hook_id = IRQ_KEYBOARD;
int keyboard_return_value = 0;
uint8_t scancode = 0;

int (keyboard_subscribe_interrupts)(uint8_t *bit_no){
  *bit_no = hook_id;
  return sys_irqsetpolicy(IRQ_KEYBOARD, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id);
}

int (keyboard_unsubscribe_interrupts)(){
  return sys_irqrmpolicy(&hook_id);
}

void (keyboard_ih)(){
  keyboard_return_value = read_KBC_output(KBC_OUT_REG, &scancode, 0);
}

int (keyboard_restore)(){
  uint8_t commandWord;
  if(write_KBC_command(KBC_IN_REG, KBC_READ_CMD_BYTE) != 0) return EXIT_FAILURE;
  if(read_KBC_output(KBC_OUT_REG, &commandWord, 0) != 0) return EXIT_FAILURE;

  commandWord |= KEYBOARD_ENABLE_INT;
  
  if(write_KBC_command(KBC_IN_REG, KBC_WRITE_CMD_BYTE) != 0) return EXIT_FAILURE;
  return write_KBC_command(KBC_OUT_REG, commandWord);
}

int (is_breakcode)(uint8_t scancode, bool *is_breakcode){
  *is_breakcode = ((BIT(7) & scancode)) >> 7;
  return EXIT_SUCCESS;
}

int (translate_scancode)(uint8_t scancode, uint8_t *character){
  for (int i = 0; i < 36; i++){
    if (scancode == makecodes[i].scancode || scancode == breakcodes[i].scancode){
      *character = makecodes[i].character;
      break;
    }
  }
  // printf("No match for scancode: 0x%x\n", scancode);
  return EXIT_SUCCESS;
}
