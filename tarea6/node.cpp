
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#include <semaphore.h>

#include "shdata.h"

/* 
   This process creates 10 children processes to do some work, and then exit

   You must first run main, which will create the shared memory
   segment and wait for results.

   In this case, each child will do a lot of meaningless processing,
   just as an example, and them put the results in shared memory.
   The shared memory key is read from a file shmkey.txt.
*/


double numbers[] = {10.9, 22.3, 22.5, 77.6, 99.1};


int main(int argc, char *argv[])
{
  int n_elements = 61;
  int n_processors = 10;

  // First calculate how much work to give to each process.
  // Really, this is just a dummy example. Each child will just
  // do some meaningless calculations as an example.
  int n_per_processor = n_elements / n_processors;

  int n_remaining = n_elements % n_processors;

  int node_start = 0;
  int node_end;
  printf("\n");

  // Here we read the key for the shared memory. This will be availble
  // to all clients so they can attach to shared memory.
  // Remember, each child will inherit an exact copy of the stack and
  // heap.
  //
  //
  int shm_fd = shm_open("/homecarrshm1", O_RDWR, 0755);
  if (shm_fd < 0)
  {
    perror("shm_open");
    exit(0);
  }
  void *mptr = mmap(NULL, getpagesize(), PROT_READ|PROT_WRITE,
                   MAP_SHARED, shm_fd,   0x0);
  if (mptr == MAP_FAILED)
  {
    perror("mmap");
    exit(0);
  }

  shdata_t *shdata = (shdata_t *) mptr;

  sem_t *sem1 = sem_open("/home.carr.sem1", 0);
  if (sem1 == SEM_FAILED)
  {
    perror("sem_open sem1");
    exit(0);
  }

  sem_t *sem2 = sem_open("/home.carr.sem2", 0);
  if (sem2 == SEM_FAILED)
  {
    perror("sem_open sem2");
    exit(0);
  }



  pid_t pid;
  for (int i=0; i < n_processors; i++)
  {
    if (n_remaining > 0)
    {
      node_end = node_start + n_per_processor;
      n_remaining--;
    }
    else
    {
      node_end = node_start + n_per_processor - 1;
    }
    // Ok, here we go, creating two processes
    pid = fork();
    if (pid == 0)
    {
      // If fork returns zero, you are a child, and you will have
      // to do some work.


      pid_t my_pid = getpid();
      printf("I have %d, working on %d to %d, my_pid: %d\n", 
		      node_end - node_start +1, node_start, node_end, my_pid);
      // Ok, now do some stupid work.
      double total = 0;
      for (int j=0; j < 10000000; j++)
      {
        total += (numbers[j%5] / 22222.5) * j + i;
      }
      printf("child %d, my pid is: %d, total = %lf\n", i, my_pid, total);
      // Write the data to shared memory.
      shdata->values[i] = total;
      sem_wait(sem2);
      shdata->n_finished++;
      if (shdata->n_finished >= 10)
      {
        shdata->n_finished = 0;
        printf("I am the very last child to do my work, I will wake up dad!\n");
        sem_post(sem1);
      }
      sem_post(sem2);
      exit(0);
    }
    node_start = node_end + 1; 
  }
  if (pid != 0)
  {
     printf("I am the father, I forked 10 children\n");
     printf("I will die now and hope they do their work\n");
  }

}
