
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#include <mqueue.h>

#include "mydata.h"

/*
This program must be run first, then the second program "node" will
create children to send messages.

This program will create a message queue, then write the id 
to a file named msgid.txt. It will then wait for 10 other processes 
to send messages, and when complete, write the
totals and a grand total to standard output.
*/

int main(int argc, char *argv[])
{

  /* First create the message que */
  mqd_t mq_fd = mq_open("/wikastarea2", O_RDWR | O_CREAT, 0755, NULL);
  if (mq_fd == ((mqd_t) -1))
  {
    perror("mq_open");
    exit(0);
  }

  struct mq_attr attr;
  int n = mq_getattr(mq_fd, &attr);
  if (n == -1)
  {
    perror("mq_getattr");
    exit(0);
  }
  printf("attr.mq_flags: %ld\n", attr.mq_flags);
  printf("attr.mq_maxmsg: %ld\n", attr.mq_maxmsg);
  printf("attr.mq_msgsize: %ld\n", attr.mq_msgsize);
  printf("attr.mq_curmsgs: %ld\n", attr.mq_curmsgs);

  attr.mq_msgsize = sizeof(mydata_t); 
  n = mq_setattr(mq_fd, &attr, NULL);
  if (n == -1)
  {
    perror("mq_setattr");
    exit(0);
  }

  n = mq_getattr(mq_fd, &attr);
  if (n == -1)
  {
    perror("mq_getattr");
    exit(0);
  }
  printf("attr.mq_flags: %ld\n", attr.mq_flags);
  printf("attr.mq_maxmsg: %ld\n", attr.mq_maxmsg);
  printf("attr.mq_msgsize: %ld\n", attr.mq_msgsize);
  printf("attr.mq_curmsgs: %ld\n", attr.mq_curmsgs);

  // Ok, now we will wait for 10 messages.
  printf("I'm on the need of CONSUMEEEE . . . . . I'll wait, tururu\n");
  printf("run \"the producer\" now\n");

  int count = 0;
  for (int i=0; i < 100; i++)
  {
    mydata_t mydata;
    int rval;

    unsigned prio;
    char msgbuf[8192];
    rval = mq_receive(mq_fd, msgbuf, sizeof(msgbuf), &prio);
    if (rval == -1)
    {
      perror("mq_recieve");
      exit(0);
    }
    memcpy(&mydata, msgbuf, sizeof(mydata));
    for (int j = 0; j < mydata.val ; j++) 
      printf("eating %d... buuuurp!!!\n", ++count);
  }

  // Finally, delete the message queue.

  int retval = mq_unlink("/wikastarea2");
  if (retval == -1)
  {
    perror("mq_send");
    exit(0);
  }
  printf("Destroy successful, retval = %d\n", retval);
  printf("Hasta la vista baby!\n");
}
