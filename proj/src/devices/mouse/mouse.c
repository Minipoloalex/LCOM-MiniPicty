#include "mouse.h"

static int hook_id = IRQ_MOUSE;
int return_value_mouse = 0;

uint8_t packet_byte = 0;
uint8_t packet_index = 0;
struct packet packet;


static bool packet_ready = false;
bool (packet_is_ready)() {
  return packet_ready;
}
uint8_t *(mouse_get_packet_bytes)() {
  return packet.bytes;
}

int (mouse_subscribe_interrupts)(uint8_t *bit_no){
  *bit_no = hook_id;
  return sys_irqsetpolicy(IRQ_MOUSE, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id);
}

int (mouse_unsubscribe_interrupts)(){
  return sys_irqrmpolicy(&hook_id);
}

int (mouse_enable_data_report)() {
  return write_to_mouse(MOUSE_ENABLE_DATA_REP);
}

int (mouse_disable_data_report)(){
  return write_to_mouse(MOUSE_DISABLE_DATA_REP);
}

void (mouse_ih)() {
  return_value_mouse = read_KBC_output(KBC_OUT_REG, &packet_byte, true);
}

int (write_to_mouse)(uint8_t command){
  uint8_t attempts = MAX_ATTEMPTS;
  uint8_t mouse_response = 0;

  while(attempts > 0 && mouse_response != MOUSE_ACK){
    if (write_KBC_command(KBC_IN_REG, KBC_WRITE_MOUSE_CMD) != 0) {
      printf("1st write_kbc_command inside %s\n", __func__);
      return EXIT_FAILURE;
    }
    if (write_KBC_command(KBC_OUT_REG, command) != 0) {
      printf("2nd write_kbc_command inside %s\n", __func__);
      return EXIT_FAILURE;
    }
    
    tickdelay(micros_to_ticks(WAIT_KBC));
    
    if (read_KBC_output(KBC_OUT_REG, &mouse_response, true)) {
      printf("read_kbc_output inside %s\n", __func__);
      return EXIT_FAILURE;
    }
    attempts--;
  }
  return mouse_response != MOUSE_ACK;
}

int (mouse_enable_int)() {
  if (sys_irqenable(&hook_id) != OK) {
    printf("sys_irqenable inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int (mouse_disable_int)() {
  if (sys_irqdisable(&hook_id) != OK) {
    printf("sys_irqdisable inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int (mouse_process_packet_byte)() {  
  if (packet_index > 2) {
    printf("index > 2 inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  
  packet.bytes[packet_index] = packet_byte;
  
  if (packet_index == 0 && (packet_byte & FIRST_BYTE_VALIDATION)) {
    packet_ready = false;

    packet.lb = LB(packet_byte);
    packet.rb = RB(packet_byte);
    packet.mb = MB(packet_byte);
    
    packet.x_ov = X_OV(packet_byte);
    packet.y_ov = Y_OV(packet_byte);

    packet.delta_x = DELTA_X(packet_byte) ? 0xFF00 : 0x0000;
    packet.delta_y = DELTA_Y(packet_byte) ? 0xFF00 : 0x0000;

    packet_index++;
  }
  else if (packet_index == 1) {
    packet.delta_x |= packet_byte;
    packet_index++;
  }
  else if (packet_index == 2) {
    packet.delta_y |= packet_byte;
    packet_index++;

    packet_ready = true;
  }
  return EXIT_SUCCESS;
}

drawing_position_t (mouse_get_drawing_position_from_packet)(position_t before_position) {
  packet_ready = false;
  packet_index = 0;
  extern unsigned h_res;
  extern unsigned v_res;
  position_t next_position = before_position;

  uint16_t new_x = before_position.x + packet.delta_x;
  uint16_t new_y = before_position.y - packet.delta_y;
  // printf("before_position.x: %d, before_position.y: %d\n", before_position.x, before_position.y);
  // printf("packet.delta_x: %d, packet.delta_y: %d\n", packet.delta_x, packet.delta_y);
  if(new_x >= 0 && new_x < h_res) next_position.x = new_x;
  if(new_y >= 0 && new_y < v_res) next_position.y = new_y;
  // printf("new_x: %d, new_y: %d\n", new_x, new_y);

  drawing_position_t drawing_position = {
    .position = next_position,
    .is_drawing = packet.lb
  };
  printf("drawing_position.x: %d, drawing_position.y: %d, drawing_position.is_drawing: %d\n", drawing_position.position.x, drawing_position.position.y, drawing_position.is_drawing);
  return drawing_position;
}

// int (mouse_process_packet)(){
//   if (mouse_get_packet() != 0) return EXIT_FAILURE;

//   if (packet_index == 3) {
//     packet_index = 0;
//     last_mouse_position = mouse_position;

//     extern unsigned h_res;
//     extern unsigned v_res;

//     uint16_t new_x = mouse_position.x + packet.delta_x;
//     if(new_x > 0 && new_x < h_res){
//       mouse_position.x = new_x;
//     }
//     uint16_t new_y = mouse_position.y - packet.delta_y;
//     if(new_y > 0 && new_y < v_res){
//       mouse_position.y = new_y;
//     }

//     if(packet.lb) {
//       drawing = true;
//     }else{
//       drawing = false;
//     }
//     // if(packet.lb) {
//     //   vg_draw_line(last_mouse_position, mouse_position, 20, color);
//     // }
//   }

//   return EXIT_SUCCESS;
// }
