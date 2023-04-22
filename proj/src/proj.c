#include <lcom/lcf.h>
#include <lcom/proj.h>

#include <stdint.h>
#include <stdio.h>

#include "modules/interrupts/interrupts.h"

int main(int argc, char *argv[]) {
  lcf_set_language("EN-US");
  lcf_trace_calls("/home/lcom/labs/proj/trace.txt");
  lcf_log_output("/home/lcom/labs/proj/output.txt");
  if (lcf_start(argc, argv))
    return 1;
  lcf_cleanup();
  return 0;
}

typedef enum {
  MENU,
  GAME,
} state_t;


int(proj_main_loop)(int argc, char *argv[]) {

  // Load resources

  // Subscribe interrupts
  if(subscribe_interrupts()) return 1;

  int ipc_status;
  message msg;
  state_t state = MENU;


  // Game Loop
  while(true){

    // Handle the user input with interrupts
    int r;
    if((r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
        printf("driver_receive failed with: %d", r);
        continue;
    }
  
    if (is_ipc_notify(ipc_status)) { 
      switch (_ENDPOINT_P(msg.m_source)) {
        //TODO: implement receiving interrupts
        default:
          break;
      }
    } else {
        /* 
          received a standard message, not a notification
          no standard messages expected: do nothing
        */
    }
  }

  // Unsubscribe interrupts
  if(unsubscribe_interrupts()) return 1;

  return 0;
}
