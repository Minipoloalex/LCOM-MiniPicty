#include "player_menu.h"

struct PlayerMenu {
  position_t last_mouse_position;
  queue_t *mouse_positions;
};


player_menu_t *(create_player_menu)() {
  player_menu_t *player_menu = malloc(sizeof(player_menu_t));
  if (player_menu == NULL) {
    return NULL;
  }
  player_menu->last_mouse_position = (position_t) {0, 0};
  player_menu->mouse_positions = create_queue(QUEUE_SIZE, sizeof(drawing_position_t));
  return player_menu;
}

void (destroy_player_menu)(player_menu_t * player_menu) {
  delete_queue(mouse_positions);
  free(player_menu);
}


/*

Francisco:
- Palete cores - 5 cores, [as cores são botões: mudam cor do player drawer brush]
- 1 Borracha
- butões (+ e -) para aumentar/diminiur tamanho
- Limitar o desenho a uma area
- botão para limpar o ecrã

Vicente:
- file with strings: current and guess
- drawing letters and strings
- Enter clears string and checks if guess is correct
- backspace removes characters from current
- sprite para os mouses: borracha, pincel, genérico

João: 
- Triple buffering

Félix:
- Process interrupt para cada estado
- Triple buffering

*/
