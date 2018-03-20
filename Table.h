#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <ncurses.h>

typedef void * (*THREADFUNCPTR)(void *);

#include "Philosopher.h"

class Table {
  private:
    pthread_t *philThreads;
    unsigned int philsNumber;
    Philosopher **phils;
  public:
    Table(unsigned int _philsNumber) {
      philsNumber = _philsNumber;
      phils = new Philosopher*[_philsNumber];
      philThreads = new pthread_t[_philsNumber];
    }
    void Run();
};
