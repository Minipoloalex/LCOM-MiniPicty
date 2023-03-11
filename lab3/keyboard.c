#include <lcom/lcf.h>

#include <stdint.h>

#include "i8042.h"

static int hook_id = KEYBOARD_IRQ;
uint8_t scancode = 0;
int return_value;

int (keyboard_subscribe_int)(uint8_t *bit_no){
  *bit_no = hook_id;
  return sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id);
}

int (keyboard_unsubscribe_int)(){
  return sys_irqrmpolicy(&hook_id);
}

extern uint8_t scancode;
extern int return_value;
void (kbc_ih)(){
  uint8_t status;
  if (util_sys_inb(KBC_STATUS_REG, &status) != 0) {
    return_value = EXIT_FAILURE;
    return;
  }

  // status might contain error: kbc_parity_err | kbc_timeout_err = 11000000
  if(status & (KBC_PARITY_ERR | KBC_TIMEOUT_ERR)){ 
    return_value = EXIT_FAILURE;
    return;
  }


  if(util_sys_inb(KBC_OUT_BUF, &scancode) != 0){
    return_value = EXIT_FAILURE;
    return;
  }
  return_value = 0;
}

int (enable_int)(){
    sys_outb(KBC_STATUS_REG, KBC_READ_CMD);
    uint8_t cmd;
    util_sys_inb(KBC_OUT_BUF, &cmd);

    cmd |= KBC_ENABLE_INT;
    if(sys_outb(KBC_CMD_REG, KBC_WRITE_CMD) != 0) return EXIT_FAILURE;

    if(sys_outb(KBC_IN_REG, cmd) != 0) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
