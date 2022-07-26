#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <vector>


using namespace std;


int mesgid=1;  // 消息的记数器。

// 缓存消息的结构体。
struct st_message
{
  int mesgid;
  char message[1024];
} stmesg;


vector<struct st_message> vcache; // 用vector容器做缓存。

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;     // 声名并初始化条件变量。
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;    // 声名并初始化互斥锁。


// 消费者、出队线程主函数。
void *outcache(void *arg)
{
  struct st_message stmesg;

  while (true)
  {
    pthread_mutex_lock(&mutex); // 加锁。

    // 如果缓存为空，等待。 // 条件变量虚假唤醒。
    while (vcache.size() == 0)
    {
      pthread_cond_wait(&cond,&mutex);
      printf("条件变量被唤醒\n");
    }

    // 从缓存中获取第一条记录，然后删除该记录。
    memcpy(&stmesg,&vcache[0],sizeof(struct st_message)); // 内存拷贝。
    vcache.erase(vcache.begin());

    pthread_mutex_unlock(&mutex);   // 解锁。

    // 以下是处理业务的代码。
    printf("phid=%ld,mesgid=%d\n",pthread_self(),stmesg.mesgid);

    usleep(100); //处理业务需要时间，没有则会给一个业务处理
  }
}



// 生产者、把生产的数据存入缓存。
void incache(int sig)
{
  struct st_message stmesg;

  memset(&stmesg,0,sizeof(struct st_message));

  // 加锁时间越短越好
  // stmesg.mesgid=mesgid++;
  // pthread_mutex_lock(&mutex); // 加锁。
  // vcache.push_back(stmesg);
  // pthread_mutex_unlock(&mutex); // 解锁。

  pthread_mutex_lock(&mutex); // 加锁。

  // 生产数据，放入缓存。
  stmesg.mesgid=mesgid++; vcache.push_back(stmesg);   // 内存拷贝。
  stmesg.mesgid=mesgid++; vcache.push_back(stmesg);
  stmesg.mesgid=mesgid++; vcache.push_back(stmesg);
  stmesg.mesgid=mesgid++; vcache.push_back(stmesg);
  stmesg.mesgid=mesgid++; vcache.push_back(stmesg);

  pthread_mutex_unlock(&mutex); // 解锁。

  pthread_cond_broadcast(&cond);  // 触发条件，激活全部的线程。
}


int main()
{
  signal(15,incache); // 接收15的信号，调用生产者函数。

  pthread_t thid1,thid2,thid3;

  pthread_create(&thid1,NULL,outcache,NULL);
  pthread_create(&thid2,NULL,outcache,NULL);
  pthread_create(&thid3,NULL,outcache,NULL);

  pthread_join(thid1,NULL);
  pthread_join(thid2,NULL);
  pthread_join(thid3,NULL);

  return 0;
}