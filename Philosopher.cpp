#include "Philosopher.h"

void Philosopher::StartPhilCycle() {
  erase();
  clear();
  refresh();
  attroff( A_BOLD | A_UNDERLINE);
  for(unsigned int j = 0; j < cycles; j++) {
    Think();
    GrabForks();
    Eat();
    PutOutForks();
  }
  move_and_lock(initInfoYPosition, initInfoXPosition, identifier);
  printw("STOP!");
  refresh_and_unlock();
}

void Philosopher::GrabLeftFork() {
    int retVal;
    struct timespec ts;
    ts = get_waiting_time(200);
    pthread_mutex_lock(&mutex);
    while(forks[leftForkId].taken) {
      refresh_forks_state();
      retVal = pthread_cond_timedwait(&cond, &mutex, &ts);
    }
    move_and_lock(initInfoYPosition + 1, initInfoXPosition, identifier);
    if(retVal == ETIMEDOUT) {
      printw("TIMEOUT - PUT OUT LEFT FORK");
      forks[leftForkId].taken = false;
      forks[leftForkId].owner = -1;
    } else {
      printw("GRABBED LEFT FORK");
      forks[leftForkId].taken = true;
      forks[leftForkId].owner = identifier;
    }
    pthread_cond_signal(&cond);
    refresh_and_unlock();
    refresh_forks_state();
    pthread_mutex_unlock(&mutex);
    SLEEP(500);
}

void Philosopher::GrabRightFork() {
    int retVal;
    struct timespec ts;
    ts = get_waiting_time(200);
    pthread_mutex_lock(&mutex);
    while(forks[rightForkId].taken) {
      refresh_forks_state();
      retVal = pthread_cond_timedwait(&cond, &mutex, &ts);
    }
    move_and_lock(initInfoYPosition + 1, initInfoXPosition, identifier);
    if(retVal == ETIMEDOUT) {
      printw("TIMEOUT - PUT OUT RIGHT FORK");
      forks[rightForkId].taken = false;
      forks[rightForkId].owner = -1;
    } else {
      printw("GRABBED RIGHT FORK");
      forks[rightForkId].taken = true;
      forks[rightForkId].owner = identifier;
    }
    pthread_cond_signal(&cond);
    refresh_and_unlock();
    refresh_forks_state();
    pthread_mutex_unlock(&mutex);
    SLEEP(500);
}

void Philosopher::DisplayState() {
  move_and_lock(initInfoYPosition, initInfoXPosition, identifier);
  switch(state) {
    case 0:
        printw("HUNGRY");
      break;
    case 1:
        green_color();
        printw("EATING");
        move(initInfoYPosition + 1, initInfoXPosition);
        clrtoeol();
        release_color();
      break;
    case 2:
        blue_color();
        printw("THINKING");
        release_color();
      break;
  }
  refresh_and_unlock();
}

void Philosopher::GrabForks() {
  GrabLeftFork();
  GrabRightFork();

  if(forks[rightForkId].taken && forks[leftForkId].taken && forks[rightForkId].owner == identifier && forks[leftForkId].owner == identifier) {
    move_and_lock(initInfoYPosition + 1, initInfoXPosition, identifier);
    printw("GRABBED TWO FORKS");
    refresh_and_unlock();
    refresh_forks_state();
  }
  SLEEP(1000);
}

void Philosopher::PutOutLeftFork() {
  forks[leftForkId].owner = -1;
  forks[leftForkId].taken = false;
  move_and_lock(initInfoYPosition + 1, initInfoXPosition, identifier);
  printw("PUT OUT LEFT FORK");
  refresh_and_unlock();
  refresh_forks_state();
}

void Philosopher::PutOutRightFork() {
  forks[rightForkId].owner = -1;
  forks[rightForkId].taken = false;
  move_and_lock(initInfoYPosition + 1, initInfoXPosition, identifier);
  printw("PUT OUT RIGHT FORK");
  refresh_and_unlock();
  refresh_forks_state();
}

void Philosopher::PutOutForks() {
  PutOutRightFork();
  pthread_cond_signal(&cond);

  PutOutLeftFork();
  pthread_cond_signal(&cond);

  move_and_lock(initInfoYPosition + 1, initInfoXPosition, identifier);
  printw("PUT OUT FORKS");
  refresh_and_unlock();
  refresh_forks_state();
  pthread_cond_signal(&cond);
  SLEEP(1000);
}

void Philosopher::Eat() {
  if(forks[rightForkId].owner == identifier &&
     forks[leftForkId].owner == identifier &&
     forks[leftForkId].taken &&
     forks[rightForkId].taken) {
    state = 1;
    DisplayState();
    for(int i = 0; i <= 5; i++) {
      move_and_lock(initInfoYPosition + 1, initInfoXPosition, identifier);
      green_color();
      printw("%d %% - [%s%s]", i * 20, std::string(i, '=').c_str(), std::string(5 - i, ' ').c_str());
      release_color();
      refresh_and_unlock();
      SLEEP(randEatingTime());
    }
  }
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
