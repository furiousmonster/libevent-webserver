#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

// 使用
// gcc mutithread.c -l pthread

int x = 0, y = 0; //x用于线程1，y用于线程2

pthread_t pthid1, pthid2;

//第一个线程的主函数
void *pth1_main(void *arg);

//第二个线程主函数
void *pth2_main(void *arg);

int main()
{
  //创建线程1
  if (pthread_create(&pthid1, NULL, pth1_main, (void*)0) != 0)
  {
    printf("pthread_create pthid1 failed. \n");
    return -1;
  }

  if (pthread_create(&pthid2, NULL, pth2_main, (void*)0) != 0)
  {
    printf("pthread_create pthid2 failed. \n");
    return -1;
  }

  printf("111\n");
  pthread_join(pthid1, NULL);

  printf("222\n");
  pthread_join(pthid2, NULL);

  return 0;
}

// 第一个线程的主函数
void *pth1_main(void *arg)
{
  // for (x = 0; x < 100; x++) {
  //   printf("x=%d\n", x);
  //   sleep(1);
  // }

  for (x = 0; x < 10; x++) {
    printf("x=%d\n", x);
    sleep(1);
  }

  pthread_exit(NULL);
}

// 第二个线程的主函数
void *pth2_main(void *arg)
{
  // for (y = 0; y < 100; y++) {
  //   printf("y=%d\n", y);
  //   sleep(1);
  // }

  //锁
  for (x = 10; x > 0; x--) {
    printf("x=%d\n", x);
    sleep(1);
  }

  pthread_exit(NULL);
}