#include "kbc.h"

int (read_kbc_status)(uint8_t *status) {
  return util_sys_inb(KBC_STATUS_REG, status);
}

int (read_kbc_output)(int port, uint8_t *output) {
  uint8_t status;
  for (int i = 0; i < MAX_ATTEMPTS; i++) {
    if (read_kbc_status(&status)) return 1;

    if ((status & KBC_OBF) && ((status & BIT(5)) == 0)) {
      if (util_sys_inb(port, output)) return 1;
      if ((status & KBC_PARITY_ERR) || (status & KBC_TIMEOUT_ERR)) return 1;
      return 0;
    }
    if (tickdelay(micros_to_ticks(WAIT_KBC))) return 1;
  }
  return 1;
}

// Write to kbc a command (0x64), or arguments to a command (0x60)
int (write_kbc_command)(int port, uint8_t command) {
  uint8_t status;
  for (int i = 0; i < MAX_ATTEMPTS; i++) {
    if (read_kbc_status(&status)) return 1;
    
    // IBF = 1 means don't write commands or arguments (0x60 or 0x64)
    if ((status & KBC_IBF) == 0) {
      if (sys_outb(port, command)) return 1;
      return 0;
    }
    if (tickdelay(micros_to_ticks(WAIT_KBC))) return 1;
  }
  return 1;
}
