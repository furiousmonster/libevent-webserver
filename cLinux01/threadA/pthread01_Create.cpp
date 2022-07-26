#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *mainfunc1(void *arg);
void *mainfunc2(void *arg);

// g++ pthread01_Create.cpp -lpthread
volatile int v = 0;

int main()
{
  pthread_t pthid1 = 0, pthid2 = 0; // typedef unsigned long int pthread_t

  // 创建一个线程
  if (pthread_create(&pthid1, NULL, mainfunc1, NULL) != 0)
  {
    printf("创建线程失败，程序退出.\n");
    return -1;
  }

  if (pthread_create(&pthid2, NULL, mainfunc2, NULL) != 0)
  {
    printf("创建线程失败，程序退出.\n");
    return -1;
  }

  printf("pthid1 = %lu, pthid2 = %lu \n", pthid1, pthid2);

  printf("等待子线程退出\n");
  pthread_join(pthid1, NULL);
  pthread_join(pthid2, NULL);
  printf("子线程已退出.\n");


  return 0;
}

void *mainfunc1(void *arg)
{
  for (int ii = 0; ii < 5; ii++) 
  {
    v++;
    sleep(1);
    printf("thread1 sleep 1sec ok. %d \n", v);
  }
}


void *mainfunc2(void *arg)
{
  for (int ii = 0; ii < 5; ii++) 
  {
    v++;
    sleep(1);
    printf("thread2 sleep 1sec ok. %d \n", v);
  }
}