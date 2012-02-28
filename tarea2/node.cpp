
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
   This process creates 10 children processes to do some work, and then exit

   You must first run main, which will create the shared memory
   segment and wait for results.

   In this case, each child will do a lot of meaningless processing,
   just as an example, and then send the total via a message queue.
   The message queue id is read from a file msgid.txt.
*/


int main(int argc, char *argv[])
{

  mqd_t mq_fd = mq_open("/wikastarea2", O_RDWR);
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

  int count = 0;
  for (int j=0; j < 100; j++)
  {
    mydata_t mydata;
    printf("Damn! the pot is empty!\n");
    mydata.val = 10.0; 
    mydata.mq_fd = 0;

    int rval = mq_send(mq_fd, (char *) &mydata, sizeof(mydata), 0);
    if (rval == -1)
    {
      perror("mq_send");
      exit(0);
    }
    printf("producing 10 more ... Total sent: %d\n", ++count*10);
  }
}
