#include "mouse.h"

static int hook_id = IRQ_MOUSE;
int return_value_mouse = 0;
uint8_t packet_byte = 0;

int (mouse_subscribe_interrupts)(uint8_t *bit_no){
  *bit_no = hook_id;
  return sys_irqsetpolicy(IRQ_MOUSE, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id);
}

int (mouse_unsubscribe_interrupts)(){
  return sys_irqrmpolicy(&hook_id);
}

int (mouse_enable_data_report)() {
  return write_to_mouse(MOUSE_ENABLE_DATA_REP);
}

int (mouse_disable_data_report)(){
  return write_to_mouse(MOUSE_DISABLE_DATA_REP);
}

void (mouse_ih)(){
  return_value_mouse = read_KBC_output(KBC_OUT_REG, &packet_byte, 1);
}
