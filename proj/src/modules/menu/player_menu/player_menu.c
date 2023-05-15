#include "player_menu.h"

struct PlayerMenu {
  player_t *player;
};


player_menu_t *(create_player_menu)() {
  player_menu_t *player_menu = malloc(sizeof(player_menu_t));
  if (player_menu == NULL) {
    return NULL;
  }
  player_menu->player = create_player();
  if (player_menu->player == NULL) {
    free(player_menu);
    return NULL;
  }
  return player_menu;
}

void (destroy_player_menu)(player_menu_t * player_menu) {
  destroy_player(player_menu->player);
}

player_t *(player_menu_get_player)(player_menu_t * player_menu) {
  return player_menu->player;
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
