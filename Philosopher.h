#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <ncurses.h>
#include <sys/time.h>
#include <errno.h>
#include <string>

#if defined(__unix__)
# include <unistd.h>
# define SLEEP(x) usleep(x * 1000)
#elif defined (WIN32) || defined(_WIN32)
# include <windows.h>
# define SLEEP(x) Sleep(x)
#endif

#define MAX_THINKING_TIME 5000
#define MIN_THINKING_TIME 2000
#define MAX_EATING_TIME 500
#define MIN_EATING_TIME 100

struct Fork{
  bool taken;
  int owner;
};

static pthread_mutex_t ncurses = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static Fork forks[] = { { false, -1 }, { false, -1 }, { false, -1 }, { false, -1 }, { false, -1 } };

class Philosopher {
  private:
    unsigned int identifier;
    unsigned int state;
    unsigned int cycles;
    unsigned int leftForkId, rightForkId;
    unsigned int leftNeighbour, rightNeighbour;
    unsigned int initInfoYPosition, initInfoXPosition;
    static int randEatingTime() {
      return (int) (MAX_EATING_TIME ? (rand() % MAX_EATING_TIME + MIN_EATING_TIME) : MIN_EATING_TIME);
    }

    static int randThinkingTime() {
      return (int) (MAX_THINKING_TIME ? (rand() % MAX_THINKING_TIME + MIN_THINKING_TIME) : MIN_EATING_TIME);
    }

    static void refresh_labels(int i) {
      //pthread_mutex_lock(&ncurses);
      move(i * 5 + 1 + 5, 0);
      clrtoeol();
      attron(A_BOLD | A_UNDERLINE);
      printw("[%d]. philosopher:", i);
      refresh();
      attroff(A_BOLD | A_UNDERLINE);
      //pthread_mutex_unlock(&ncurses);
    }

    static void move_and_lock(int y, int x, int i) {
      pthread_mutex_lock(&ncurses);
      refresh_labels(i);
      move(y, x);
      clrtoeol();
    }

    static void refresh_and_unlock() {
      refresh();
      pthread_mutex_unlock(&ncurses);
    }

    static timespec get_waiting_time(int ms) {
      struct timespec ts;
      struct timeval tv;
      gettimeofday(&tv, NULL);
      ts.tv_sec = time(NULL);
      ts.tv_nsec = tv.tv_usec * 1000 + 1000 * 1000 * (ms % 1000);
      ts.tv_sec += ts.tv_nsec / (1000 * 1000 * 1000);
      ts.tv_nsec %= (1000 * 1000 * 1000);
      return ts;
    }

    static void green_color() {
      start_color();
      init_pair(1, COLOR_GREEN, COLOR_BLACK);
      attron(COLOR_PAIR(1));
    }

    static void blue_color() {
      start_color();
      init_pair(1, COLOR_BLUE, COLOR_BLACK);
      attron(COLOR_PAIR(1));
    }

    static void release_color() {
      attroff(COLOR_PAIR(1));
    }

    static void refresh_forks_state() {
      for(int i = 0; i < 5; i++) {
        pthread_mutex_lock(&ncurses);
        move(i, 0);
        clrtoeol();
        printw("Fork %d - owner: %d", i, forks[i].owner);
        refresh();
        pthread_mutex_unlock(&ncurses);
      }
    }
  public:
    void DisplayState();
    void GrabForks();
    void PutOutForks();
    void Eat();
    void Think();
    void StartPhilCycle();
    void GrabRightFork();
    void GrabLeftFork();
    void PutOutLeftFork();
    void PutOutRightFork();
    int GetIdentifier();
    Philosopher() {}
    Philosopher(unsigned int _identifier, unsigned int _state, unsigned int _cycles, unsigned int _drawWidth, unsigned int _drawHeight) {
      identifier = _identifier;
      state = _state;
      cycles = _cycles;

      initInfoYPosition = _drawHeight;
      initInfoXPosition = _drawWidth;

      if((_identifier + 1) >= 5) rightForkId = 0;
      else rightForkId = _identifier + 1;

      rightNeighbour = rightForkId;

      leftForkId = _identifier;

      leftNeighbour = leftForkId;
    }
};
