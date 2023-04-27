#include "interrupts.h"

// Devices bit numbers
uint8_t mouse_bit_no;
uint8_t keyboard_bit_no;

int(subscribe_interrupts)(){  
  
  // Mouse
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

  // Keyboard
  if(keyboard_subscribe_interrupts(&keyboard_bit_no) != 0) {
    printf("keyboard_subscribe_interrupts inside %s\n", __func__);
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int(unsubscribe_interrupts)(){

  // Mouse
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

  // Keyboard
  if(keyboard_unsubscribe_interrupts() != 0) return EXIT_FAILURE;

  return EXIT_SUCCESS;
}
