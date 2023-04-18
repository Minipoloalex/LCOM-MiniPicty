#include "interrupts.h"

//Declare the bit numbers here
static uint8_t mouse_bit_no = 0;

int(subscribe_interrupts)(){  
  
  //mouse
  if(mouse_subscribe_interrupts(&mouse_bit_no) != 0) {
    printf("mouse_subscribe_interrupts inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if(mouse_disable_int() != 0) {
    printf("mouse_disable_int inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if(mouse_enable_data_report() != 0) {
    printf("mouse_enable_data_report inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if(mouse_enable_int() != 0) {
    printf("mouse_enable_int inside %s\n", __func__);
    return EXIT_FAILURE;
  }

  return 0;
}

int(unsubscribe_interrupts)(){

  //mouse
  if(mouse_disable_int()) {
    printf("mouse_disable_int inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if(mouse_disable_data_report() != 0) {
    printf("mouse_disable_data_report inside %s\n", __func__);
    return EXIT_FAILURE;    
  }

  if(mouse_enable_int() != 0) {
    printf("mouse_enable_int inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if(mouse_unsubscribe_interrupts() != 0) return EXIT_FAILURE; 

  return 0;
}
