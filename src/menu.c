#include "lib.h"

enum MenuOption { PLAY, QUIT };

void menu_select(WINDOW *win, enum MenuOption opt) {
  int play_color = opt == PLAY ? 9 : 8;
  int quit_color = opt == QUIT ? 9 : 8;

  wattron(win, COLOR_PAIR(play_color));
  mvwprintw(win, LINES / 2 - 1, COLS / 2 - 4, "Play");
  wattroff(win, COLOR_PAIR(play_color));

  wattron(win, COLOR_PAIR(quit_color));
  mvwprintw(win, LINES / 2 - 1, COLS / 2 + 1, "Quit");
  wattroff(win, COLOR_PAIR(quit_color));

  wrefresh(win);
}

void print_menu(WINDOW *win, Vec highscores) {
  wclear(win);
  wrefresh(win);

  char *quit_msg = "q (quit)";
  mvwprintw(win, LINES - 1, (COLS - strlen(quit_msg)) / 2, quit_msg);

  char *legend = "Use arrow keys to select left/right";
  mvwprintw(win, LINES - 2, (COLS - strlen(legend)) / 2, legend);

  char *title[] = {" _______  _______  _______  ______    ___   _______ \n",
                   "|       ||       ||       ||    _ |  |   | |       |\n",
                   "|_     _||    ___||_     _||   | ||  |   | |  _____|\n",
                   "  |   |  |   |___   |   |  |   |_||_ |   | | |_____ \n",
                   "  |   |  |    ___|  |   |  |    __  ||   | |_____  |\n",
                   "  |   |  |   |___   |   |  |   |  | ||   |  _____| |\n",
                   "  |___|  |_______|  |___|  |___|  |_||___| |_______|\n"};

  for (int i = 0; i < 7; i++) {
    mvwprintw(win, i + LINES / 2 - 11, (COLS - strlen(title[i])) / 2, title[i]);
  }

  menu_select(win, PLAY);

  int last_score = 0;
  if (highscores.length != 0) {
    last_score = vec_get_last(highscores);
  }

  int n_digits_highscore = last_score == 0 ? 0 : log10(last_score) + 1;
  int highscore_row_size = 16 + n_digits_highscore;

  mvwprintw(win, LINES / 2 + 3, (COLS - highscore_row_size) / 2,
            "Previous score: %i", last_score);

  // print highscores
  mvwprintw(win, LINES / 2 + 5, (COLS - 10) / 2, "Highscores");

  vec_sort(&highscores);
  vec_reverse(&highscores);

  int n = highscores.length > 10 ? 10 : highscores.length;

  for (int i = 0; i < n; i++) {
    int score = vec_get(highscores, i);

    mvwprintw(win, LINES / 2 + 6 + i, (COLS - 10) / 2, "%i", score);
  }

  wrefresh(win);
}

void menu(enum State *game_state, Vec *highscores) {
  WINDOW *win = newwin(0, 0, 0, 0);

  print_menu(win, *highscores);

  enum MenuOption menu_opt = PLAY;

  bool run = true;
  while (run) {
    // handle keypress
    int ch = getch();

    switch (ch) {
    case 113: // q
      menu_opt = QUIT;
      run = false;
      break;
    case 10: // enter
      run = false;
      break;
    case KEY_LEFT:
      menu_select(win, PLAY);
      menu_opt = PLAY;
      break;
    case KEY_RIGHT:
      menu_select(win, QUIT);
      menu_opt = QUIT;
      break;
    }
  }

  wclear(win);
  wrefresh(win);
  delwin(win);

  if (menu_opt == QUIT) {
    quit();
  } else {
    *game_state = GAME;
  }
}
