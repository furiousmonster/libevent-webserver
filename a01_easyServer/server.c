#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
 
#define SERV_PORT 8000

int main(void) {
        
        int lfd, cfd;
        struct sockaddr_in serv_addr, clin_addr;
        socklen_t clin_len; // 客户端返回数据长度
        char buf[1024];
        int len;

        // 使用AF_INET:iPv4, SOCK_STREAM：stream协议 ,默认0:tcp
        // lfd是服务器ip地址的索引
        lfd = socket(AF_INET, SOCK_STREAM, 0);

        // 构造IP地址
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); // 绑定任意网卡
        serv_addr.sin_port = htons(SERV_PORT); // host to 网络序 -- 大端存储（高地址存低地址，低地址存高地址）

        // 绑定：IP + PORT 泛型指针 类似void*
        bind(lfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

        // 监听
        listen(lfd, 128);


        // 服务器接受客户端信息
        clin_len = sizeof(clin_addr);
        cfd = accept(lfd, (struct sockaddr*)&clin_addr, &clin_len);
        len = read(cfd, buf, sizeof(buf)); // 读取信息
        // 标准输出信息
        write(STDOUT_FILENO, buf, len);

        close(lfd);
        close(cfd);
        return 0;
}