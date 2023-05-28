#include "interrupts.h"

// Devices bit numbers
uint8_t timer_bit_no;
uint8_t mouse_bit_no;
uint8_t keyboard_bit_no;
uint8_t ser_bit_no;
uint8_t rtc_bit_no;

int(subscribe_interrupts)(){  
  // Timer
  if(timer_subscribe_int(&timer_bit_no) != 0){
    printf("timer_subscribe_in inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  // RTC
  if (rtc_subscribe_int(&rtc_bit_no) != 0) {
    printf("rtc_subscribe_int inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  // Keyboard
  if(keyboard_subscribe_interrupts(&keyboard_bit_no) != 0) {
    printf("keyboard_subscribe_interrupts inside %s\n", __func__);
    return EXIT_FAILURE;
  }
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
  // Serial Port
  if (ser_subscribe_int(&ser_bit_no) != 0) {
    printf("ser_subscribe_int inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int(unsubscribe_interrupts)(){
  // Timer
  if(timer_unsubscribe_int() != 0){
    printf("timer_unsubscribe_int inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  // RTC
  if (rtc_unsubscribe_int() != 0) {
    printf("rtc_subscribe_int inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  // Serial Port
  if (ser_unsubscribe_int() != 0) {
    printf("ser_unsubscribe_int inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  // Mouse
  if(mouse_disable_int() != 0) {
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
  if(mouse_unsubscribe_interrupts() != 0) {
    printf("mouse_unsubscribe_interrupts inside %s\n", __func__);
    return EXIT_FAILURE; 
  }
 // Keyboard
  if(keyboard_unsubscribe_interrupts() != 0) {
    printf("keyboard_unsubscribe_interrupts inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  if(keyboard_restore() != 0) {
    printf("keyboard_restore inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
