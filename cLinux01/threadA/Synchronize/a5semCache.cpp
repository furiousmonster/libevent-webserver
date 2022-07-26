#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <vector>
#include <semaphore.h>

using namespace std;


int mesgid=1;  // 消息的记数器。

// 缓存消息的结构体。
struct st_message
{
  int mesgid;
  char message[1024];
} stmesg;


vector<struct st_message> vcache; // 用vector容器做缓存。

sem_t sem; //声明信号量
pthread_mutex_t mutex; //声明并初始化锁

// 消费者、出队线程主函数。
void *outcache(void *arg)
{
  struct st_message stmesg;

  while (true)
  {
    // 如果缓存为空，等待。
    while (vcache.size() == 0)
    {
      sem_wait(&sem);
      printf("%ld wait ok\n", pthread_self());
    }

    pthread_mutex_lock(&mutex); // 加锁。

    if (vcache.size() == 0) //判断缓存是否有数据
    {
      pthread_mutex_unlock(&mutex);
      continue;
    }

    // 从缓存中获取第一条记录，然后删除该记录。
    memcpy(&stmesg, &vcache[0], sizeof(struct st_message)); // 内存拷贝。
    vcache.erase(vcache.begin());

    pthread_mutex_unlock(&mutex);   // 解锁。

    // 以下是处理业务的代码。
    printf("phid=%ld,mesgid=%d\n",pthread_self(),stmesg.mesgid);

    usleep(100);
  }
}



// 生产者、把生产的数据存入缓存。
void incache(int sig)
{
  struct st_message stmesg;

  memset(&stmesg, 0, sizeof(struct st_message));

  pthread_mutex_lock(&mutex); // 加锁。

  // 生产数据，放入缓存。
  stmesg.mesgid=mesgid++; vcache.push_back(stmesg);   // 内存拷贝。
  stmesg.mesgid=mesgid++; vcache.push_back(stmesg);
  stmesg.mesgid=mesgid++; vcache.push_back(stmesg);
  stmesg.mesgid=mesgid++; vcache.push_back(stmesg);
  stmesg.mesgid=mesgid++; vcache.push_back(stmesg);

  pthread_mutex_unlock(&mutex); // 解锁。

  sem_post(&sem); //信号加1
  sem_post(&sem); //信号加1
  sem_post(&sem); //信号加1
  sem_post(&sem); //信号加1
  sem_post(&sem); //信号加1
  sem_post(&sem); //信号加1
}


int main()
{
  signal(15, incache); // 接收15的信号，调用生产者函数。

  sem_init(&sem, 0, 0); //初始化信号量
  pthread_mutex_init(&mutex, NULL); //初始化互斥锁

  pthread_t thid1,thid2,thid3;

  pthread_create(&thid1,NULL,outcache,NULL);
  pthread_create(&thid2,NULL,outcache,NULL);
  pthread_create(&thid3,NULL,outcache,NULL);

  pthread_join(thid1,NULL);
  pthread_join(thid2,NULL);
  pthread_join(thid3,NULL);

  return 0;
}