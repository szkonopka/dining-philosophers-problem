#include "Philosophers.h"

int main() {
  Philosophers phs(5);
  printf("I'm starting!\n");
  phs.Run();
  return 0;
}
