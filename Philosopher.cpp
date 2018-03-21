#include "Philosopher.h"

void Philosopher::StartPhilCycle() {
  refresh();
  attroff( A_BOLD | A_UNDERLINE);
  for(unsigned int j = 0; j < cycles; j++) {
    Think();
    pthread_mutex_lock(&mutex);
    GrabForks();
    pthread_mutex_unlock(&mutex);
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
    ts = get_waiting_time(20000);
    while(forks[leftForkId].taken) {
      //pthread_cond_wait(&cond, &mutex);
      retVal = pthread_cond_timedwait(&cond, &mutex, &ts);
    }
    //mvprintw(identifier, 0, "[%d] Philosopher grabbed left fork - %d.\n", identifier, leftForkId);
    //refresh();
    //printf("[%d] Philosopher grabbed left fork - %d.\n", identifier, leftForkId);
    move_and_lock(initInfoYPosition + 2, initInfoXPosition, identifier);
    printw("RETVAL: %d", retVal);
    refresh_and_unlock();
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
    refresh_and_unlock();
    SLEEP(1000);
}

void Philosopher::GrabRightFork() {
    int retVal;
    struct timespec ts;
    ts = get_waiting_time(20000);
    while(forks[rightForkId].taken) {
      //pthread_cond_wait(&cond, &mutex);
      retVal = pthread_cond_timedwait(&cond, &mutex, &ts);
    }
    //mvprintw(identifier, 0, "[%d] Philosopher grabbed right fork - %d.\n", identifier, rightForkId);
    //refresh();
    //printf("[%d] Philosopher grabbed right fork - %d.\n", identifier, rightForkId);
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
    refresh_and_unlock();
    SLEEP(1000);
}

void Philosopher::DisplayState() {
  //int x, y;
  //getyx(stdscr, y, x);
  //move(identifier, 0);
  //move(x, y);
  move_and_lock(initInfoYPosition, initInfoXPosition, identifier);
  switch(state) {
    case 0:
        //mvprintw(identifier, 0, "[%d] Philosopher is HUNGRY.\n", identifier);
        //printf("[%d] Philosopher is HUNGRY.\n", identifier);
        //printw("Philosopher %d is HUNGRY", identifier);
        printw("HUNGRY");
      break;
    case 1:
        //mvprintw(identifier, 0, "[%d] Philosopher is EATING.\n", identifier);
        //printf("[%d] Philosopher is EATING.\n", identifier);
        //printw("Philosopher %d is EATING", identifier);
        green_color();
        printw("EATING");
        move(initInfoYPosition + 1, initInfoXPosition);
        clrtoeol();
        release_color();
      break;
    case 2:
        //mvprintw(identifier, 0, "[%d] Philosopher is THINKING.\n", identifier);
        //printf("[%d] Philosopher is THINKING.\n", identifier);
        blue_color();
        printw("THINKING");
        release_color();
      break;
  }
  refresh_and_unlock();
  //refresh();
}

void Philosopher::GrabForks() {
  GrabLeftFork();
  GrabRightFork();

  if(forks[rightForkId].taken && forks[leftForkId].taken && forks[rightForkId].owner == identifier && forks[leftForkId].owner == identifier) {
    move_and_lock(initInfoYPosition + 1, initInfoXPosition, identifier);
    printw("GRABBED TWO FORKS");
    refresh_and_unlock();
  }

  for(int i = 0; i < 5; i++) {
    pthread_mutex_lock(&ncurses);
    move(i, 0);
    clrtoeol();
    printw("Fork %d - owner: %d", i, forks[i].owner);
    refresh();
    pthread_mutex_unlock(&ncurses);
  }
  //printf("[%d] Philosopher grabbed forks.\n", identifier);
  SLEEP(1000);
}

void Philosopher::PutOutLeftFork() {

}

void Philosopher::PutOutRightFork() {

}

void Philosopher::PutOutForks() {
  forks[rightForkId].owner = -1;
  forks[rightForkId].taken = false;
  forks[leftForkId].owner = -1;
  forks[leftForkId].taken = false;
  move_and_lock(initInfoYPosition + 1, initInfoXPosition, identifier);
  printw("PUT OUT FORKS");
  refresh_and_unlock();
  //printf("[%d] Philosopher put out forks.\n", identifier);
  pthread_cond_broadcast(&cond);
  SLEEP(1000);
}

void Philosopher::Eat() {
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

void Philosopher::Think() {
  state = 2;
  DisplayState();
  SLEEP(randThinkingTime());
  //state = 0;
  //DisplayState();
}

int Philosopher::GetIdentifier() {
  return identifier;
}
