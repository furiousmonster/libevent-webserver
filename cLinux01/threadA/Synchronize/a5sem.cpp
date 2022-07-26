#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>

char buffer[101];

sem_t sem;

void *pthfun(void *arg)
{
  for (int ii = 0; ii < 3; ii++)
  {
    printf("%d:%ld:lock...\n", time(0), (long)arg);
    sem_wait(&sem); //加锁 sem-1
    printf("%d:%ld:lock ok\n", time(0), (long)arg);

    // 操作全局变量
    sprintf(buffer, "%d:%%ld,%d", time(0), pthread_self(), ii);
    sleep(5);

    sem_post(&sem); //解锁 sem+1
    printf("%d:%ld:unlock...\n", time(0), (long)arg);

    usleep(100);

  }
}

int main()
{
  sem_init(&sem, 0, 1); //初始化信号量为1，1为开

  int val;
  sem_getvalue(&sem, &val);
  printf("val = %d\n", val);

  pthread_t pthid1, pthid2;
  pthread_create(&pthid1, NULL, pthfun, (void *)1);
  pthread_create(&pthid2, NULL, pthfun, (void *)2);

  pthread_join(pthid1, NULL);
  pthread_join(pthid2, NULL);

  sem_destroy(&sem);

  return 0;

}