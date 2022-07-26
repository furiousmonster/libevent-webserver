#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>


pthread_mutex_t mutex;
pthread_cond_t cond;

/**
 * ps -ef | grep ./a.out
 * killall -15 25164
 * 
 * 
 * 
 * 
 */ 

void *thread1(void *arg) {
  while(1) {

    pthread_cond_wait(&cond, &mutex);
    printf("线程1被唤醒\n");
  }
}

void *thread2(void *arg) {
  while(1) {

    pthread_cond_wait(&cond, &mutex);
    printf("线程2被唤醒\n");
  }
}

void func(int sig) {
  pthread_cond_signal(&cond);
  //pthread_cond_broadcast(&cond);
}

int main(){
  signal(15, func); //设置信号执行函数

  pthread_mutex_init(&mutex, NULL); //初始化锁和条件变量
  pthread_cond_init(&cond, NULL);

  pthread_t thid1, thid2;
  pthread_create(&thid1, NULL, thread1, NULL);
  pthread_create(&thid2, NULL, thread2, NULL);

  pthread_join(thid1, NULL);
  pthread_join(thid2, NULL);


  return 0;
}