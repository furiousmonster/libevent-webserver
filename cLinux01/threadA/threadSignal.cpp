#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>

void *mainfunc(void *arg);
void hdfunc(int sig);

int main()
{
  signal(2, hdfunc);

  pthread_t pthid;

  if (pthread_create(&pthid, NULL, mainfunc, NULL) != 0)
  {
    printf("failed to create thread\n");
    return -1;
  }

  // char str[5];
  // scanf("%s", str);
  // printf("%s\n", str);

  int iret, ival;
  printf("等待子进程退出\n");
  iret = pthread_join(pthid, (void**)&ival);
  printf("子线程已退出(iret=%d, ival=%d). \n", iret, ival);

  return 0;
}

void *mainfunc(void *arg)
{

  for (int ii = 0; ii < 10; ii++)
  {
    sleep(1);
    printf("sleep %dsec ok \n", ii+1);
  }

  pthread_exit(0);
  // return 0;
}

void hdfunc(int sig)
{
  printf("sig=%d\n", sig);

  for (int jj = 1; jj <= 3; jj++)
  {
    printf("hdfunc jj(%d) = %d \n", sig, jj);
    sleep(1);
  }
}