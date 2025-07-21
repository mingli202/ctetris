#include "game/game.c"
#include "menu.c"
#include "vec.h"

int main(int argc, char **argv) {
  int initial_level = 1;
  bool is_constant_level = false;

  parse_args(argc, argv, &initial_level, &is_constant_level);

  init();

  enum Screen game_state = MENU;

  Vec highscores = vec_with_capacity(10);

  while (true) {
    switch (game_state) {
    case MENU:
      menu(&game_state, &highscores);
      break;
    case GAME:
      game(&game_state, &highscores, initial_level, is_constant_level);
      break;
    }
  }

  quit();
}
