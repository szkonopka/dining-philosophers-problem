#include <ncurses.h>

int main(int argc, char **argv) {
  initscr();
  printw("Siemka");
  refresh();
  getch();
  endwin();
  return 0;
}
