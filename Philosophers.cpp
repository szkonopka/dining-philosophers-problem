#include "Philosophers.h"

void Philosophers::Run() {
  for(unsigned int i = 0; i < this->philsNumber; i++) {
    philsArgs[i].philIndex = i;
    philsArgs[i].cycles = 5 + i;
    printf("Created [%d] thread!\n", i);
    int result = pthread_create(&philThreads[i], NULL, StartPhilCycle, &philsArgs[i]);
    if(result != 0) {
      fprintf(stderr, "Error: creating thread failed.\n");
    }
  }

  for(unsigned int i = 0; i < this->philsNumber; i++) {
    void *retval = NULL;
    pthread_join(philThreads[i], &retval);
    printf("[%d] thread has closed and returned code %p\n", i, retval);
  }
}
