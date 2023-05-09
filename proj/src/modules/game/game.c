#include "game.h"

static player_drawer_t *player_drawer;
static canvas_t *canvas;

int (setup_game)() {
  // TODO: change this to depend on isTransmitter: the transmitter always starts the game drawing
  player_drawer = create_player_drawer(SELF_PLAYER);
  if (player_drawer == NULL) {
    printf("create_player_drawer inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  canvas = canvas_init();
  if (canvas == NULL) {
    destroy_player_drawer(player_drawer);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

void (destroy_game)() {
  destroy_player_drawer(player_drawer);
  canvas_destroy(canvas);
  // clear screen? double/triple buffering
}

int (game_process_mouse)() {
  player_t *player = player_drawer_get_player(player_drawer);
  drawing_position_t next = mouse_get_drawing_position_from_packet(player_get_current_position(player));
  if (player_drawer_get_state(player_drawer) == SELF_PLAYER) {  
    // if I'm the drawer, I must communicate what I'm doing
    ser_add_position_to_transmitter_queue(next);
    return player_add_next_position(player, &next);
  }
  return EXIT_SUCCESS;
}


int (game_process_serial)() {
  if (player_drawer_get_state(player_drawer) == OTHER_PLAYER) {
    ser_read_bytes_from_receiver_queue(player_drawer);
  }
  return EXIT_SUCCESS;
}

int (draw_game)(){
  //draw canvas
  //draw buttons
  //draw mouse
  if (draw_to_canvas(canvas, player_drawer) != OK) {
    printf("draw_to_canvas inside %s\n", __func__);
    return EXIT_FAILURE;
  }

  
  //if (vg_draw_player_drawer(player_drawer) != OK) return EXIT_FAILURE;
  
  // Flip Page here
  if (vg_buffer_flip()) {
    printf("vg_buffer_flip inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
