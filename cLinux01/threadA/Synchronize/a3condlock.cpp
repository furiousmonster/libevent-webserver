#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>


pthread_mutex_t mutex;
pthread_cond_t cond;


void *thread1(void *arg)
{
  while(1)
  {
    pthread_mutex_lock(&mutex);
    printf("线程一开始等待条件变量\n");
    pthread_cond_wait(&cond, &mutex);
    printf("线程1被唤醒\n");
    pthread_mutex_unlock(&mutex);
  }
}

void *thread2(void *arg)
{
  while(1)
  {
    pthread_mutex_lock(&mutex);
    printf("线程二开始等待条件变量\n");
    pthread_cond_wait(&cond, &mutex);
    printf("线程2被唤醒\n");
    pthread_mutex_unlock(&mutex);
  }
}

void func(int sig)
{
  pthread_cond_signal(&cond);
  //pthread_cond_broadcast(&cond);
}

int main(){
  signal(15, func);

  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&cond, NULL);

  pthread_t thid1, thid2;
  pthread_create(&thid1, NULL, thread1, NULL);
  pthread_create(&thid2, NULL, thread2, NULL);

  pthread_join(thid1, NULL);
  pthread_join(thid2, NULL);


  return 0;
}
