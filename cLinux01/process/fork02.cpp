#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
 
int ii=10;
 
int main()
{
  int jj=20;
 
  if (fork()>0)
  {
    ii=11;jj=21; sleep(1);  printf("父进程：ii=%d,jj=%d\n",ii,jj);
  }
  else
  {
    ii=12;jj=22; sleep(1);  printf("子进程：ii=%d,jj=%d\n",ii,jj);
  }
}