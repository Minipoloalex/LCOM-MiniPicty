#include "kbc.h"

uint8_t output = 0;

int (read_KBC_status)(uint8_t *status){
  return(util_sys_inb(KBC_STATUS_REG, status));
}

int (read_KBC_output)(int8_t port, uint8_t* output, bool isMouse){
  uint8_t status;
  uint8_t attempts = MAX_ATTEMPTS;

  while(attempts){
    if(read_KBC_status(&status)){
      printf("read_KBC_status inside %s\n", __func__);
      return EXIT_FAILURE;
    }

    if(status & FULL_OUT_BUF){
      if(util_sys_inb(port, output)){
        printf("util_sys_inb inside %s\n", __func__);
        return EXIT_FAILURE;
      }
      if((status & PARITY_ERR) || (status & TIMEOUT_ERR)){
        printf("Parity or Timeout error inside %s\n", __func__);
        return EXIT_FAILURE;
      }
      if(isMouse && !(status & AUX)) {
        printf("status & AUX error inside %s: mouse output not found\n", __func__);
        return EXIT_FAILURE;
      }
      if(!isMouse && (status & AUX)) {
        printf("status & AUX error inside %s: keyboard output not found\n", __func__);
        return EXIT_FAILURE;
      }
      return EXIT_SUCCESS;
    }
    attempts--;
    tickdelay(micros_to_ticks(WAIT_KBC));
  }
  printf("Excedeed time while waiting inside %s\n", __func__);
  return EXIT_FAILURE;
}

int (write_KBC_command)(uint8_t port, uint8_t cmd_byte){
  uint8_t status;
  uint8_t attempts = MAX_ATTEMPTS;

  while(attempts){
    if(read_KBC_status(&status)){
      printf("read_KBC_status inside %s\n", __func__);
      return EXIT_FAILURE;
    }
    if((status & FULL_IN_BUF) == 0){
      if(sys_outb(port, cmd_byte)){
        printf("sys_outb inside %s\n", __func__);
        return EXIT_FAILURE;
      }
      return EXIT_SUCCESS;
    }
    attempts--;
    tickdelay(micros_to_ticks(WAIT_KBC));
  }
  printf("Excedeed time while waiting inside %s\n", __func__);
  return EXIT_FAILURE;
}

