#include "Philosopher.h"

void Philosopher::StartPhilCycle() {
  //erase();
  //clear();
  for(unsigned int j = 0; j < cycles; j++) {
    Think();
    pthread_mutex_lock(&mutex);
    GrabForks();
    pthread_mutex_unlock(&mutex);
    Eat();
    PutOutForks();
    //refresh();
  }
}

void Philosopher::GrabLeftFork() {
    //mvprintw(identifier, 0, "[%d] Philosopher grabbed left fork - %d.\n", identifier, leftForkId);
    //refresh();
    while(forks[leftForkId].taken) {
      pthread_cond_wait(&cond, &mutex);
    }
    forks[leftForkId].taken = true;
    printf("[%d] Philosopher grabbed left fork - %d.\n", identifier, leftForkId);
    SLEEP(200);
}

void Philosopher::GrabRightFork() {
    //mvprintw(identifier, 0, "[%d] Philosopher grabbed right fork - %d.\n", identifier, rightForkId);
    //refresh();
    while(forks[rightForkId].taken) {
      pthread_cond_wait(&cond, &mutex);
    }
    forks[rightForkId].taken = true;
    printf("[%d] Philosopher grabbed right fork - %d.\n", identifier, rightForkId);
    SLEEP(200);
}

void Philosopher::DisplayState() {
  //int x, y;
  //getyx(stdscr, y, x);
  //move(identifier, 0);
  //clrtoeol();
  //move(x, y);
  switch(state) {
    case 0:
        //mvprintw(identifier, 0, "[%d] Philosopher is HUNGRY.\n", identifier);
        printf("[%d] Philosopher is HUNGRY.\n", identifier);
      break;
    case 1:
        //mvprintw(identifier, 0, "[%d] Philosopher is EATING.\n", identifier);
        printf("[%d] Philosopher is EATING.\n", identifier);
      break;
    case 2:
        //mvprintw(identifier, 0, "[%d] Philosopher is THINKING.\n", identifier);
        printf("[%d] Philosopher is THINKING.\n", identifier);
      break;
  }

  /*
  for(int x = 0; x < 5; x++) {
    if(!forks[x]) {
      mvprintw(x + 7, 0, "[%d] fork is free.", x);
    } else {
      mvprintw(x + 7, 0, "[%d] fork is blocked.", x);
    }

  }*/
  //refresh();
}

void Philosopher::GrabForks() {
  GrabLeftFork();
  GrabRightFork();
  //mvprintw(identifier, 0, "[%d] Philosopher grabbed forks.\n", identifier);
  //refresh();
  printf("[%d] Philosopher grabbed forks.\n", identifier);
  SLEEP(200);
}

void Philosopher::PutOutForks() {
  //mvprintw(identifier, 0, "[%d] Philosopher put out forks.\n", identifier);
  //refresh();
  forks[rightForkId].taken = false;
  forks[leftForkId].taken = false;
  printf("[%d] Philosopher put out forks.\n", identifier);
  pthread_cond_broadcast(&cond);
  SLEEP(200);
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
  //state = 0;
  //DisplayState();
}

int Philosopher::GetIdentifier() {
  return identifier;
}
