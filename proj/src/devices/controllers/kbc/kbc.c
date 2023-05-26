#include "kbc.h"

uint8_t output = 0;

int (read_KBC_status)(uint8_t *status){
  return(util_sys_inb(KBC_STATUS_REG, status));
}

int (read_KBC_output)(int8_t port, uint8_t* output, uint8_t mouse){
  uint8_t status;
  uint8_t attempts = MAX_ATTEMPTS;

  while(attempts){
    attempts--;
    if(read_KBC_status(&status)){
      printf("read_kbc_status inside %s\n", __func__);
      return EXIT_FAILURE;
    }
    if(mouse && !(status & AUX)) {
      printf("expected mouse int but got keyboard int inside %s\n", __func__);
      read_KBC_output_to_trash();
      tickdelay(micros_to_ticks(WAIT_KBC));
      continue;
    }
    if(!mouse && (status & AUX)) {
      printf("expected keyboard int but got mouse int inside %s\n", __func__);
      read_KBC_output_to_trash();
      tickdelay(micros_to_ticks(WAIT_KBC));
      continue;
    }

    if(status & FULL_OUT_BUF){
      if(util_sys_inb(port, output)){
        printf("util_sys_inb inside %s\n", __func__);
        return EXIT_FAILURE;
      }
      if((status & PARITY_ERR) | (status & TIMEOUT_ERR)){
        printf("Parity or timeout error inside %s\n", __func__);
        return EXIT_FAILURE;
      }
      return EXIT_SUCCESS;
    }
    tickdelay(micros_to_ticks(WAIT_KBC));
  }
  printf("excedeed attempts inside %s\n", __func__);
  return EXIT_FAILURE;
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

int (read_KBC_output_to_trash)() {
  uint8_t trash;
  util_sys_inb(KBC_OUT_REG, &trash);
  // printf("Got trash inside kbc: %02x\n", trash);
  return EXIT_SUCCESS;
}
