#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <ncurses.h>

#if defined(__unix__)
# include <unistd.h>
# define SLEEP(x) usleep(x * 1000)
#elif defined (WIN32) || defined(_WIN32)
# include <windows.h>
# define SLEEP(x) Sleep(x)
#endif

#define MAX_THINKING_TIME 2000
#define MIN_THINKING_TIME 1000
#define MAX_EATING_TIME 2000
#define MIN_EATING_TIME 1000

struct PhilArgs {
  unsigned int state;
  unsigned int philIndex;
  unsigned int cycles;
};

class Philosophers {
  private:
    pthread_t *philThreads;
    unsigned int philsNumber;
    PhilArgs *philsArgs;

    static void DisplayStates(PhilArgs i) {
      int x, y;
      getyx(stdscr, y, x);
      move(i.philIndex, 0);
      clrtoeol();
      move(x, y);
      switch(i.state) {
        case 0:
            //printf("[%d] Philosoper is HUNGRY.\n", i.philIndex);
            mvprintw(i.philIndex, 0, "[%d] Philosopher is HUNGRY.\n", i.philIndex);
          break;
        case 1:
            //printf("[%d] Philosoper is EATING.\n", i.philIndex);
            mvprintw(i.philIndex, 0, "[%d] Philosopher is EATING.\n", i.philIndex);
          break;
        case 2:
            //printf("[%d] Philosoper is THINKING.\n", i.philIndex);
            mvprintw(i.philIndex, 0, "[%d] Philosopher is THINKING.\n", i.philIndex);
          break;
      }
      refresh();
    }

    static int randEatingTime() {
      return (int) (MAX_EATING_TIME ? (rand() % MAX_EATING_TIME + MIN_EATING_TIME) : MIN_EATING_TIME);
    }

    static int randThinkingTime() {
      return (int) (MAX_THINKING_TIME ? (rand() % MAX_THINKING_TIME + MIN_THINKING_TIME) : MIN_EATING_TIME);
    }

    static void GrabForks(PhilArgs i) {
      // printf("[%d] philosopher grabbed forks.\n", i.philIndex);
      mvprintw(i.philIndex, 0, "[%d] Philosopher grabbed forks.\n", i.philIndex);
      SLEEP(1000);
      refresh();
    }

    static void PutOutForks(PhilArgs i) {
      // printf("[%d] philosopher put out forks.\n", i.philIndex);
      mvprintw(i.philIndex, 0, "[%d] Philosopher put out forks.\n", i.philIndex);
      refresh();
      SLEEP(1000);
    }

    static void Eat(PhilArgs i) {
      i.state = 1;
      DisplayStates(i);
      // printf("[%d] philosopher is eating.\n", i.philIndex);
      SLEEP(randEatingTime());
    }

    static void Think(PhilArgs i) {
      i.state = 2;
      DisplayStates(i);
      // printf("[%d] philosopher is thinking.\n", i.philIndex);
      SLEEP(randThinkingTime());
      i.state = 0;
      DisplayStates(i);
    }

    static void *StartPhilCycle(void *i) {
      PhilArgs vars = *(PhilArgs *) (i);
      erase();
      clear();
      for(unsigned int j = 0; j < vars.cycles; j++) {
        GrabForks(vars);
        Eat(vars);
        PutOutForks(vars);
        Think(vars);
        refresh();
      }
    }
  public:
    Philosophers(unsigned int _philsNumber) {
      philsNumber = _philsNumber;
      philThreads = new pthread_t[_philsNumber];
      philsArgs = new PhilArgs[_philsNumber];
    }
    void Run();
};
