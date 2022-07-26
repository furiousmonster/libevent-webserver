//互斥锁 多线程资源抢占
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

char buffer[101]; //全局共享buffer

// pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; //声明互斥锁
pthread_mutex_t mutex;

/**
 * g++ a1lock.cpp -lpthread
 * 1.   线程1 lock->lock ok;
 *      线程2lock,阻塞在pthread_mutex_lock(&mutex);
 * 2.   线程1执行操作.. 线程1解锁unlock ok
 * 3.   线程2lock ok
 *      线程1lock,阻塞在pthread_mutex_lock(&mutex);
 */
void *pthfun(void *arg)
{
  for (int ii = 0; ii < 3; ii++)
  {
    printf("%d:%ld:lock...\n", time(0), (long)arg);
    pthread_mutex_lock(&mutex); //加锁
    printf("%d:%ld:lock ok .\n", time(0), (long)arg);

    //操作全局变量
    sprintf(buffer, "%d:%ld, %d", time(0), pthread_self(), ii);
    sleep(5);
    printf(buffer);
    printf("\n");

    pthread_mutex_unlock(&mutex); //解锁
    printf("%d:%ld:unlock ok .\n", time(0), (long)arg);

    usleep(100);
  }
}

int main()
{
  pthread_mutex_init(&mutex, NULL); //初始化锁

  pthread_t pthid1, pthid2;
  pthread_create(&pthid1, NULL, pthfun, (void *)1);
  pthread_create(&pthid2, NULL, pthfun, (void *)2);

  pthread_join(pthid1, NULL);
  pthread_join(pthid2, NULL);

  pthread_mutex_destroy(&mutex); //销毁锁

  return 0;
}