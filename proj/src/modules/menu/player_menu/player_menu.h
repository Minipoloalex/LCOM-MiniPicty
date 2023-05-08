#include "../../../model/position/position.h"
#include "../../../model/drawing_position.h"
#include "../../../model/queue/queue.h"

typedef struct PlayerMenu player_menu_t;

player_menu_t *(create_player_menu)();
void (destroy_player_menu)(player_menu_t * player_menu);

