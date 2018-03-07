#include "Philosophers.h"

int main(int argc, char **argv) {
  int amount;
  char *p = argv[1];
  sscanf(argv[1], "%d", &amount);
  Philosophers phs(amount);
  phs.Run();
  return 0;

}
