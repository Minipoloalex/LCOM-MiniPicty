#include "game.h"

PlayerDrawer_t *player_drawer;

int (setup_game)() {
  // TODO: change this to depend on isTransmitter: the transmitter always starts the game drawing
  player_drawer = create_player_drawer(SELF_PLAYER);
  if (player_drawer == NULL) {
    printf("create_player_drawer inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

void (destroy_game)() {
  destroy_player_drawer(player_drawer);
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
  if (vg_draw_player_drawer(player_drawer) != OK) return EXIT_FAILURE;
  return EXIT_SUCCESS;
}
