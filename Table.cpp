#include "Table.h"

void Table::Run() {
  initscr();
  noecho();
  erase();
  clear();
  for(unsigned int i = 0; i < this->philsNumber; i++) {
    phils[i] = new Philosopher(i, 1, 5 + i);
    int result = pthread_create(&philThreads[i], NULL, (THREADFUNCPTR) &Philosopher::StartPhilCycle, phils[i]);
    if(result != 0) {
      fprintf(stderr, "Error: creating thread failed.\n");
    }
  }

  for(unsigned int i = 0; i < this->philsNumber; i++) {
    void *retval = NULL;
    pthread_join(philThreads[i], &retval);
    mvprintw(phils[i]->GetIdentifier(), 0, "[%d] Philosoper has ENDED and RETURNED CODE: %p.\n", phils[i]->GetIdentifier(), retval);
    refresh();
  }
  endwin();
}
