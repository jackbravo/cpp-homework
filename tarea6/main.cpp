
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
This program must be run first, then the second program "node" will
create children to write data to shared memory.

This program will create a shared memory segment, then write the key
to a file named shmkey.txt. It will then wait for 10 other processes 
to write totals to the shared memory, and when complete, write the
totals and a grand total to standard output.
*/

int main(int argc, char *argv[])
{
  int shm_fd = shm_open("/wikastarea6", O_CREAT | O_RDWR | O_TRUNC , 0755);
  if (shm_fd < 0)
  {
    perror("shm_open");
    exit(0);
  }

  int ret_val = ftruncate(shm_fd, sizeof(shdata_t));  
  if (ret_val < 0)
  {
    perror("ftruncate");
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

  shdata->n_finished = 0;

  int semval, retval;

  sem_t *sem1 = sem_open("/wikassem1", O_CREAT, 0755, 0);
  if (sem1 == SEM_FAILED)
  {
    perror("sem_open sem1");
    exit(0);
  }

  sem_t *sem2 = sem_open("/wikassem2", O_CREAT, 0755, 1);
  if (sem2 == SEM_FAILED)
  {
    perror("sem_open sem2");
    exit(0);
  }

  retval = sem_getvalue(sem1, &semval);
  if (retval != 0)
  {
    perror("sem_getval sem1");
    exit(0);
  }
  printf("sem1 value = %d\n", semval);

  retval = sem_getvalue(sem2, &semval);
  if (retval != 0)
  {
    perror("sem_getval sem2");
    exit(0);
  }
  printf("sem2 value = %d\n", semval);


  // Ok, now we wait on a semaphore 
  printf("Waiting for results . . . . . (run \"node\" now)\n");

  sem_wait(sem1);

  printf("Woooiii!! The results are in, here they are:\n");
  double grand_total = 0.0;
  for (int i=0;  i < 10; i++)
  {
    printf("value[%d]: %lf\n", i, shdata->values[i]);
    grand_total += shdata->values[i];
  }
  printf("Grand Total: %lf\n", grand_total);

  printf("Cleaning up! Sending semaphores to the garbage\n");
  retval = sem_unlink("/wikassem1");
  if (retval != 0)
  {
    perror("sem_unlink sem1");
  }
  
  retval = sem_unlink("/wikassem2");
  if (retval != 0)
  {
    perror("sem_unlink sem2");
  }

}

