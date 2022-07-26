#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
 
//xx pthread_mutex_t mutex; // 申明一个互斥锁
 
// 与客户端通信线程的主函数
void *pth_main(void *arg)
{
  int pno=(long)arg;   // 线程编号
 
  pthread_detach(pthread_self());
 
  char strbuffer[1024];
 
  for (int ii=0;ii<3;ii++)    // 与服务端进行3次交互。
  {
    //xx pthread_mutex_lock(&mutex);  // 加锁
    memset(strbuffer,0,sizeof(strbuffer));
    sprintf(strbuffer,"线程%d：这是第%d个超级女生，编号%03d。",pno,ii+1,ii+1);
    if (TcpClient.Send(strbuffer,strlen(strbuffer))<=0) break;
    printf("发送：%s\n",strbuffer);
 
    memset(strbuffer,0,sizeof(strbuffer));
    if (TcpClient.Recv(strbuffer,sizeof(strbuffer))<=0) break;
    printf("线程%d接收：%s\n",pno,strbuffer);
    //xx pthread_mutex_unlock(&mutex);  // 释放锁
    // usleep(100);   // usleep(100)，否则其它的线程无法获得锁。
  }
 
  pthread_exit(0);
}
 
int main()
{
  // CTcpClient TcpClient;
  // 向服务器发起连接请求
  if (TcpClient.ConnectToServer("172.16.0.15",5051)==false)
  { printf("TcpClient.ConnectToServer(\"172.16.0.15\",5051) failed,exit...\n"); return -1; }
 
  //xx pthread_mutex_init(&mutex,0); // 创建锁
 
  pthread_t pthid1,pthid2;
  pthread_create(&pthid1,NULL,pth_main,(void*)1);   // 创建第一个线程
  pthread_create(&pthid2,NULL,pth_main,(void*)2);   // 创建第二个线程
 
  pthread_join(pthid1,NULL);    // 等待线程1退出。
  pthread_join(pthid2,NULL);    // 等待线程2退出。
 
  //xx pthread_mutex_lock(&mutex);   // 销毁锁
}