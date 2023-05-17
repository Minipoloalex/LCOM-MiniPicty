#include "game.h"

static player_drawer_t *player_drawer;
static canvas_t *canvas;
static guess_word_t *guess;

int (setup_game)(bool isTransmitter) {
  player_drawer = create_player_drawer(isTransmitter ? SELF_PLAYER : OTHER_PLAYER);
  if (player_drawer == NULL) {
    printf("create_player_drawer inside %s\n", __func__);
    return EXIT_FAILURE;
  }
  canvas = canvas_init();
  guess = create_guess_word();
  if (canvas == NULL || guess == NULL) {
    destroy_player_drawer(player_drawer);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

void (destroy_game)() {
  destroy_player_drawer(player_drawer);
  canvas_destroy(canvas);
  destroy_guess_word(guess);
  vg_clear_buffers();
}

extern int keyboard_return_value;
extern uint8_t scancode;
int (game_process_keyboard)(){
  if (player_drawer_get_state(player_drawer) == SELF_PLAYER) return EXIT_SUCCESS;
  if (keyboard_return_value) return EXIT_SUCCESS;

  if (scancode == MAKE_BACKSPACE){
    delete_character(guess);
  }
  else if (scancode == MAKE_ENTER){
    bool right;
    char *correct = "door"; //TODO passar palavra certa
    validate_guess_word(correct, guess, &right);
    printf("%d \n", right);
    reset_guess_word(guess);

    //TODO se acertar, avançar
  }
  else {
    bool is_break = false;
    if (is_breakcode(scancode, &is_break)) return EXIT_FAILURE;

    if (!is_break){
      uint8_t caracter = 0;
      if (translate_scancode(scancode, &caracter)) return EXIT_FAILURE;
      if (caracter != 0){
        if (write_character(guess, caracter)) return EXIT_FAILURE;
      }
    }
  }
  return EXIT_SUCCESS;
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
  if (/*buffers_need_update()*/ true) {
    // copy do canvas para o buffer
    if (vg_copy_canvas_buffer(get_buffer(canvas))) {
      printf("vg_copy_canvas_buffer inside %s\n", __func__);
      return EXIT_FAILURE;
    }
    //printf("%d\n", guess->pointer);
    if (vg_draw_guess(guess, GUESS_POS_X, GUESS_POS_Y) != OK){
      return EXIT_FAILURE;
    }
    if (vg_buffer_flip()) {
      printf("vg_buffer_flip inside %s\n", __func__);
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}
