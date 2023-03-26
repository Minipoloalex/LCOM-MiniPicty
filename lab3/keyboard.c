#include "keyboard.h"

uint8_t scancode = 0;
int return_value = 0;
void (kbc_ih)() {
  return_value = read_kbc_output(KBC_OUT_REG, &scancode);
}

static int hook_id = IRQ_KEYBOARD;
int (keyboard_subscribe_int)(uint8_t *bit_no) {
  *bit_no = hook_id;
  return sys_irqsetpolicy(IRQ_KEYBOARD, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id);
}

int (keyboard_unsubscribe_int)() {
  return sys_irqrmpolicy(&hook_id);
}

int (keyboard_enable_int)() {
  if (write_kbc_command(KBC_CMD_REG, KBC_READ_CMD_BYTE)) return 1;
  uint8_t command;
  if (read_kbc_output(KBC_OUT_REG, &command)) return 1;
  command |= KEYBOARD_ENABLE_INT;
  if (write_kbc_command(KBC_CMD_REG, KBC_WRITE_CMD_BYTE)) return 1;
  return write_kbc_command(KBC_OUT_REG, command);
}
