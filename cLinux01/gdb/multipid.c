#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

//include ‘<stdlib.h>’ or provide a declaration of ‘exit’
//include ‘<stdio.h>’ or provide a declaration of ‘printf’


int main()
{
  printf("begin\n");

  if (fork() != 0)
  {
    printf("我是父进程：pid=%d, ppid=%d\n", getpid(), getppid());

    int ii;
    for (ii = 0; ii < 10; ii++) {
      printf("ii=%d\n", ii);
      sleep(1);
    }

    exit(0);
  }
  else
  {
    printf("我是子进程：pid=%d, ppid=%d\n", getpid(), getppid());

    int jj;
    for (jj = 0; jj < 10; jj++) {
      printf("jj=%d\n", jj);
      sleep(1);
    }

    exit(0);
  }


}