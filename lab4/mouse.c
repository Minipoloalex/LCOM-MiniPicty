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
  return_value_mouse = read_KBC_output(KBC_OUT_REG, &packet_byte, true);
}

int (write_to_mouse)(uint8_t command){
  uint8_t attempts = MAX_ATTEMPTS;
  uint8_t mouse_response = 0;

  while(attempts > 0 && mouse_response != MOUSE_ACK){

    if (write_KBC_command(KBC_IN_REG, KBC_WRITE_MOUSE_CMD) != 0) {
      printf("1st write_kbc_command inside %s\n", __func__);
      return EXIT_FAILURE;
    }
    if (write_KBC_command(KBC_OUT_REG, command) != 0) {
      printf("2nd write_kbc_command inside %s\n", __func__);
      return EXIT_FAILURE;
    }
    
    tickdelay(micros_to_ticks(WAIT_KBC));
    
    if (read_KBC_output(KBC_OUT_REG, &mouse_response, true)) {
      printf("read_kbc_output inside %s\n", __func__);
      return EXIT_FAILURE;
    }
    attempts--;
  }
  return EXIT_SUCCESS;
}


int (mouse_enable_int)() {
  if (sys_irqenable(&hook_id) != OK) {
    printf("sys_irqenable inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int (mouse_disable_int)() {
  if (sys_irqdisable(&hook_id) != OK) {
    printf("sys_irqdisable inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
