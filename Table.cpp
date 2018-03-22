#include "Table.h"

static void draw_philosopher(int i, int amount) {
  int row, col;
  getmaxyx(stdscr, row, col);
  col = 0;
  move(i * 5 + 2 + amount, col);
  clrtoeol();
  attron( A_BOLD | A_UNDERLINE );
  printw("[%d]. philosopher:", i);
  refresh();
}

void Table::Run() {
  initscr();
  noecho();
  erase();
  clear();
  refresh();
  int row, col;
  getmaxyx(stdscr, row, col);
  for(int i = 0; i < this->philsNumber; i++) {
    phils[i] = new Philosopher(i, 1, 5, 0, i * 5 + 2 + philsNumber);
    draw_philosopher(i, philsNumber);
    int result = pthread_create(&philThreads[i], NULL, (THREADFUNCPTR) &Philosopher::StartPhilCycle, phils[i]);
    if(result != 0) {
      //fprintf(stderr, "Error: creating thread failed.\n");
      return;
    }
  }

  for(unsigned int i = 0; i < this->philsNumber; i++) {
    void *retval = NULL;
    pthread_join(philThreads[i], &retval);
    //mvprintw(phils[i]->GetIdentifier(), 0, "[%d] Philosoper has ENDED and RETURNED CODE: %p.\n", phils[i]->GetIdentifier(), retval);
    //printf("[%d] Philosoper has ENDED and RETURNED CODE: %p.\n", phils[i]->GetIdentifier(), retval);
    //refresh();
  }
  getch();
  endwin();
}
