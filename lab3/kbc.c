#include "kbc.h"

void (delay)(uint8_t *attemps){
    (*attemps)--;
    tickdelay(micros_to_ticks(WAIT_KBC));
}

int (read_KBC_status)(uint8_t *status){
  return(util_sys_inb(KBC_STATUS_REG, status));
}

int (read_KBC_output)(int8_t port, uint8_t* output){
  /*
  1- Verificar se o output buffer está cheio
  2- Verificar erros de Paridade ou Timeout
  3- Se sim, lê-se
  */

  uint8_t status;

  uint8_t attemps = MAX_ATTEMPTS;
  for (int i = 0; i < 5; i++){

    // error reading status    
    if(read_KBC_status(&status)){
      return EXIT_FAILURE;
    }
        // 1
    if((status & FULL_OUT_BUF) && ((status & BIT(5)) == 0)){
      
      // 3
      if(util_sys_inb(port, output)){
        return EXIT_FAILURE;
      }

      // 2
      if((status & PARITY_ERR) | (status & TIMEOUT_ERR)){
        return EXIT_FAILURE;
      }

      return EXIT_SUCCESS;
    }

    attemps--;
    tickdelay(micros_to_ticks(WAIT_KBC));
  }

  return EXIT_SUCCESS;
}

int (write_KBC_command)(uint8_t port, uint8_t cmd_byte){
  uint8_t status;
  uint8_t attemps = MAX_ATTEMPTS;

  while(attemps){

    /*
    1- Verificar se o input buffer está cheio
    2- Se não, escreve-se
    */

    // error reading status
    if(read_KBC_status(&status)){
      return EXIT_FAILURE;
    }

    // 1
    if((status & FULL_IN_BUF) == 0){
      
      // 2
      if(sys_outb(port, cmd_byte)){
        return EXIT_FAILURE;
      }

      return EXIT_SUCCESS;
    }

    delay(&attemps);
  }

  return EXIT_FAILURE;
}
