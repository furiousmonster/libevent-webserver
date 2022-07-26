//自旋锁
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

char buffer[101]; //全局共享buffer

pthread_spinlock_t spin; //声明自旋锁

void *pthfun(void *arg)
{
  for (int ii = 0; ii < 3; ii++)
  {
    printf("%d:%ld:lock...\n", time(0), (long)arg);
    pthread_spin_lock(&spin); //加锁
    printf("%d:%ld:lock ok .\n", time(0), (long)arg);

    //操作全局变量
    sprintf(buffer, "%d:%ld, %d", time(0), pthread_self(), ii);
    sleep(5);

    pthread_spin_unlock(&spin); //解锁
    printf("%d:%ld:unlock ok .\n", time(0), (long)arg);

    usleep(100);
  }
}

int main()
{
  pthread_spin_init(&spin, NULL); //初始化锁

  pthread_t pthid1, pthid2;
  pthread_create(&pthid1, NULL, pthfun, (void *)1);
  pthread_create(&pthid2, NULL, pthfun, (void *)2);

  pthread_join(pthid1, NULL);
  pthread_join(pthid2, NULL);

  pthread_spin_destroy(&spin); //销毁锁

  return 0;
}