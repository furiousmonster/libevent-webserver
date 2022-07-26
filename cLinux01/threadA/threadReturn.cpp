#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void *mainfunc(void *arg);

int main()
{
  pthread_t pthid;

  if (pthread_create(&pthid, NULL, mainfunc, NULL) != 0)
  {
    printf("failed to create thread\n");
    return -1;
  }

  int ival;
  printf("等待子进程退出\n");
  int iret = pthread_join(pthid, (void**)&ival);
  printf("子线程已退出(iret=%d, ival=%d). \n", iret, ival);

  return 0;
}


void *mainfunc(void *arg)
{
  for (int ii = 0; ii < 3; ii++)
  {
    sleep(1);
    printf("sleep %dsec ok \n", ii+1);
  }

  pthread_exit(0);
  // return 0;
  // return (void *)10;
  // pthread_exit((void*)10);

}