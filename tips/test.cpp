// tcpserver_epoll.c

#include<sys/epoll.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<sys/socket.h>
#include<mysql/mysql.h>


int main(){
	
	//创建套接字
	int fd=socket(AF_INET,SOCK_STREAM,0);
	
	//绑定
	struct sockaddr_in addr;
	addr.sin_family=AF_INET;
	addr.sin_port=htons(8081);
	inet_pton(AF_INET,"192.168.37.3",&addr.sin_addr.s_addr);
	bind(fd,(struct sockaddr *)&addr,sizeof(addr));
	
	//监听
	listen(fd,128);	
	
	//创建epoll树
	int efd=epoll_create(1);
	
	//上树
	struct epoll_event ev,evs[1024];
	ev.data.fd=fd;
	ev.events=EPOLLIN;
	epoll_ctl(efd,EPOLL_CTL_ADD,fd,&ev);
	
	while(1){
		
		int nready=epoll_wait(efd,evs,1024,-1);
		if(nready<0){
			perror("epoll_wait");
			break;
		
		}else if(nready > 0){//有文件描述符有变化
			int i;
			for(i=0;i<nready;i++){
				if(evs[i].data.fd==fd && evs[i].events & EPOLLIN){
					
					//创建提取对象
					struct 	sockaddr_in clientAddr;
					socklen_t len=sizeof(clientAddr);
					
					//提取连接
					int cfd=accept(fd,(struct sockaddr *)&clientAddr,&len);	
										
					//输出客户端连接信息
					char ip[16];
					printf("new client ip=%s;port=%d\n",inet_ntop(AF_INET,&clientAddr.sin_addr.s_addr,ip,16),ntohs(clientAddr.sin_port));
					
					//上树
					ev.data.fd=cfd;
					ev.events=EPOLLIN;
					epoll_ctl(efd,EPOLL_CTL_ADD,cfd,&ev);
							
				}else if(evs[i].events & EPOLLIN){
					
					char buf[1024]="";
					int n = read(evs[i].data.fd,buf,sizeof(buf));
					
					if(n<0){//出错，移出
							
						perror("read");
						close(evs[i].data.fd);
						epoll_ctl(efd,EPOLL_CTL_DEL,evs[i].data.fd,&evs[i]);
						
					}else if(n == 0){//关闭，移出
						
						printf("client close\n");
						close(evs[i].data.fd);
						epoll_ctl(efd,EPOLL_CTL_DEL,evs[i].data.fd,&evs[i]);
						
					}else{
						printf("%s\n",buf);
						write(evs[i].data.fd,buf,n);
						
					}
							
				}
				
			}
			
		}
		
	}
	
	return 0;	
}
