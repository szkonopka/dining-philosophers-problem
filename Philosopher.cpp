#include "Philosopher.h"

void Philosopher::StartPhilCycle() {
  erase();
  clear();
  for(unsigned int j = 0; j < cycles; j++) {
    if(!forks[(identifier - 1)  % 5]) {
      forks[(identifier - 1) % 5] = true;
      mvprintw(identifier, 0, "[%d] Philosopher grabbed left fork.\n", identifier);
    }

    if(!forks[(identifier + 1) % 5]) {
      forks[(identifier + 1) % 5] = true;
      mvprintw(identifier, 0, "[%d] Philosopher grabbed right fork.\n", identifier);
    }

    pthread_mutex_lock(&mutex);
    if(forks[forks[(identifier + 1) % 5]] && forks[forks[(identifier - 1) % 5]]) {
      GrabForks();
      Eat();
      PutOutForks();
      pthread_cond_signal(&cond);
      Think();
    } else {
      pthread_cond_wait(&cond, &mutex);
      refresh();
    }
    pthread_mutex_unlock(&mutex);
  }
}

void Philosopher::DisplayState() {
  int x, y;
  getyx(stdscr, y, x);
  move(identifier, 0);
  clrtoeol();
  move(x, y);
  switch(state) {
    case 0:
        mvprintw(identifier, 0, "[%d] Philosopher is HUNGRY.\n", identifier);
      break;
    case 1:
        mvprintw(identifier, 0, "[%d] Philosopher is EATING.\n", identifier);
      break;
    case 2:
        mvprintw(identifier, 0, "[%d] Philosopher is THINKING.\n", identifier);
      break;
  }
  refresh();
}

void Philosopher::GrabForks() {
  mvprintw(identifier, 0, "[%d] Philosopher grabbed forks.\n", identifier);
  SLEEP(1000);
  refresh();
}

void Philosopher::PutOutForks() {
  forks[(identifier + 1) % 5] = false;
  forks[(identifier - 1) % 5] = false;
  mvprintw(identifier, 0, "[%d] Philosopher put out forks.\n", identifier);
  refresh();
  SLEEP(1000);
}

void Philosopher::Eat() {
  state = 1;
  DisplayState();
  SLEEP(randEatingTime());
}

void Philosopher::Think() {
  state = 2;
  DisplayState();
  SLEEP(randThinkingTime());
  state = 0;
  DisplayState();
}

int Philosopher::GetIdentifier() {
  return identifier;
}
