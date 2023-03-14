#include "mouse.h"

static int hook_id = IRQ_MOUSE;
int return_value_mouse = 0;
extern int output;

int (mouse_subscribe_interrupts)(uint8_t *bit_no){
  *bit_no = hook_id;
  return sys_irqsetpolicy(IRQ_MOUSE, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id);
}

int (mouse_unsubscribe_interrupts)(){
  return sys_irqrmpolicy(&hook_id);
}

int (mouse_disable_data_reporting)(){
  if(write_KBC_command(KBC_CMD_REG, KBC_WRITE_B_TO_MOUSE) != 0) return EXIT_FAILURE;
  if(write_KBC_command(KBC_OUT_REG, MOUSE_DISABLE_DATA_REP) != 0) return EXIT_FAILURE;
  return EXIT_SUCCESS;
}

void (mouse_ih)(){
  return_value_mouse = kbc_ih(1);
}
