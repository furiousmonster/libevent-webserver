#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void *mainfunc(void *arg);

void clearfunc1(void *arg);
void clearfunc2(void *arg);
void clearfunc3(void *arg);


int main()
{
  pthread_t pthid;

  if (pthread_create(&pthid, NULL, mainfunc, NULL) != 0)
  {
    printf("failed to create thread\n");
    return -1;
  }

  int iret, ival;
  printf("等待子进程退出\n");
  iret = pthread_join(pthid, (void**)&ival);
  printf("子线程已退出(iret=%d, ival=%d). \n", iret, ival);

  return 0;
}


void *mainfunc(void *arg)
{
  pthread_cleanup_push(clearfunc1, NULL);
  pthread_cleanup_push(clearfunc2, NULL);
  pthread_cleanup_push(clearfunc3, NULL);
  for (int ii = 0; ii < 3; ii++)
  {
    sleep(1);
    printf("sleep %dsec ok \n", ii+1);
  }

  // pthread_exit(0);
  // return 0;

  pthread_cleanup_pop(1);
  pthread_cleanup_pop(1);
  pthread_cleanup_pop(1);
  
}

void clearfunc1(void *arg)
{
  printf("call clearfunc1. \n");
}

void clearfunc2(void *arg)
{
  printf("call clearfunc2. \n");
}

void clearfunc3(void *arg)
{
  printf("call clearfunc3. \n");
}