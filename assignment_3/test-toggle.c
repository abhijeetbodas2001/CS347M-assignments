#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <wait.h>
#include <pthread.h>

#include "zemaphore.h"

#define NUM_THREADS 3
#define NUM_ITER 10

zem_t zems[NUM_THREADS];

void *justprint(void *data)
{
  int thread_id = *((int *)data);

  zem_down(&zems[thread_id]);
  for(int i=0; i < NUM_ITER; i++)
    {
      printf("This is thread %d\n", thread_id);
      zem_up(&zems[(thread_id+1)%NUM_THREADS]);

      if (i != NUM_ITER-1) {
        // Don't go to sleep when completing the loop
        zem_down(&zems[thread_id]);
      }

    }

  return 0;
}

int main(int argc, char *argv[])
{

  pthread_t mythreads[NUM_THREADS];
  int mythread_id[NUM_THREADS];

  
  for(int i =0; i < NUM_THREADS; i++)
    {
      mythread_id[i] = i;
      zem_init(&zems[i], 0);
      pthread_create(&mythreads[i], NULL, justprint, (void *)&mythread_id[i]);
    }

  zem_up(&zems[0]);
  
  for(int i =0; i < NUM_THREADS; i++)
    {
      pthread_join(mythreads[i], NULL);
    }
  
  return 0;
}
