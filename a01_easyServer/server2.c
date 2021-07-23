#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
 
#define SERVER_PORT 9996

int main()
{
    int listen_fd = socket(AF_INET,SOCK_STREAM,0);
    int conn_fd = 0;
    char data[] = "Hello World!";
 
    struct sockaddr_in server_addr;
    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(SERVER_PORT);
    
    bind(listen_fd,(struct sockaddr *)&server_addr,sizeof(server_addr));
    listen(listen_fd,32);
    
    while(1)
    {
        conn_fd = accept(listen_fd, (struct sockaddr *)NULL, NULL);
        write(conn_fd, data, strlen(data));
        close(conn_fd);
    }
    close(listen_fd);

    return 0;
}