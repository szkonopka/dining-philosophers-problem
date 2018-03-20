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

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static bool forks[5] = { false, false, false, false, false };

class Philosopher {
  private:
    unsigned int identifier;
    unsigned int state;
    unsigned int cycles;
    static int randEatingTime() {
      return (int) (MAX_EATING_TIME ? (rand() % MAX_EATING_TIME + MIN_EATING_TIME) : MIN_EATING_TIME);
    }

    static int randThinkingTime() {
      return (int) (MAX_THINKING_TIME ? (rand() % MAX_THINKING_TIME + MIN_THINKING_TIME) : MIN_EATING_TIME);
    }

  public:
    void DisplayState();
    void GrabForks();
    void PutOutForks();
    void Eat();
    void Think();
    void StartPhilCycle();
    int GetIdentifier();
    Philosopher() {}
    Philosopher(unsigned int _identifier, unsigned int _state, unsigned int _cycles) {
      identifier = _identifier;
      state = _state;
      cycles = _cycles;
      mutex = PTHREAD_MUTEX_INITIALIZER;
    }
};
