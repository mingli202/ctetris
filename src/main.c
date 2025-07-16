#include "game/game.c"
#include "menu.c"

int main(int argc, char **argv) {
  int initial_level = 1;

  if (argc == 2) {
    initial_level = atoi(argv[1]);
  }

  init();

  enum State game_state = MENU;

  Vec highscores = vec_new(10);

  while (true) {
    switch (game_state) {
    case MENU:
      menu(&game_state, &highscores);
      break;
    case GAME:
      game(&game_state, &highscores, initial_level);
      break;
    }
  }

  quit();
}
