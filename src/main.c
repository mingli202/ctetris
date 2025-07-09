#include "game/game.c"
#include "menu.c"

int main(void) {
  init();

  enum State game_state = MENU;

  Vec highscores = vec_new(10);

  while (true) {
    switch (game_state) {
    case MENU:
      menu(&game_state, &highscores);
      break;
    case GAME:
      game(&game_state, &highscores);
      break;
    }
  }

  quit();
}
