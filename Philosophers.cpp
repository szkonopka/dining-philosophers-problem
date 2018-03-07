#include "Philosophers.h"

void Philosophers::Run() {
  initscr();
  noecho();
  erase();
  clear();
  for(unsigned int i = 0; i < this->philsNumber; i++) {
    philsArgs[i].state = 1;
    philsArgs[i].philIndex = i;
    philsArgs[i].cycles = 5 + i;
    //printf("Created [%d] thread!\n", i);
    int result = pthread_create(&philThreads[i], NULL, StartPhilCycle, &philsArgs[i]);
    if(result != 0) {
      fprintf(stderr, "Error: creating thread failed.\n");
    }
  }

  for(unsigned int i = 0; i < this->philsNumber; i++) {
    void *retval = NULL;
    pthread_join(philThreads[i], &retval);
    //printf("[%d] thread has closed and returned code %p\n", i, retval);
    mvprintw(philsArgs[i].philIndex, 0, "[%d] Philosoper has ENDED and RETURNED CODE: %p.\n", philsArgs[i].philIndex, retval);
    refresh();
  }
  endwin();
}
