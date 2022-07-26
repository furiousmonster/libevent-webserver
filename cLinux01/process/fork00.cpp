#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
 
int main()
{
  printf("本程序的进程编号是：%d\n",getpid());
  printf("父进程会执行这句话\n");
 
  int ipid=fork();
 
  sleep(1);       // sleep等待进程的生成。
 
  printf("pid=%d\n",ipid);
 
  if (ipid!=0) printf("父进程编号是：%d\n",getpid());
  else printf("子进程编号是：%d\n",getpid());
 
  sleep(30);    // 是为了方便查看进程在shell下用ps -ef|grep book252查看本进程的编号。
}