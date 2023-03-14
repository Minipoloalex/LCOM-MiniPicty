#include "kbc.h"

uint8_t output = 0;

int (read_KBC_status)(uint8_t *status){
  return(util_sys_inb(KBC_STATUS_REG, status));
}

int (read_KBC_output)(int8_t port, uint8_t* output, uint8_t mouse){
  uint8_t status;
  uint8_t attempts = MAX_ATTEMPTS;

  while(attempts){
    if(read_KBC_status(&status)){
      return EXIT_FAILURE;
    }

    if(mouse && !(status & AUX)) return EXIT_FAILURE;
    if(!mouse && (status & AUX)) return EXIT_FAILURE;

    if(status & FULL_OUT_BUF){
      if(util_sys_inb(port, output)){
        return EXIT_FAILURE;
      }
      if((status & PARITY_ERR) | (status & TIMEOUT_ERR)){
        return EXIT_FAILURE;
      }
      return EXIT_SUCCESS;
    }
    attempts--;
    tickdelay(micros_to_ticks(WAIT_KBC));
  }
  return EXIT_SUCCESS;
}

int (write_KBC_command)(uint8_t port, uint8_t cmd_byte){
  uint8_t status;
  uint8_t attempts = MAX_ATTEMPTS;

  while(attempts){
    if(read_KBC_status(&status)){
      return EXIT_FAILURE;
    }
    if((status & FULL_IN_BUF) == 0){
      if(sys_outb(port, cmd_byte)){
        return EXIT_FAILURE;
      }
      return EXIT_SUCCESS;
    }
    attempts--;
    tickdelay(micros_to_ticks(WAIT_KBC));
  }

  return EXIT_FAILURE;
}

int (kbc_ih)(uint8_t mouse){
  return read_KBC_output(KBC_OUT_REG, &output, mouse);
}
