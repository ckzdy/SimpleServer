#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
#include "t_net.h"
#include "handle.h"
//父进程收到该信号，回收子进程的资源
void doit(int n){
    waitpid(-1, NULL, WNOHANG);
    return;
}
int main(){
    int cfd;//用于和客户端通讯的描述符
    char IP[64];
    //声明一个ipv4地址家族类型的变量，它是具体的通信地址，包括本机的ip地址和端口号
    SA4 cli;
    signal(SIGCHLD, doit);
    //调用s_bind函数
    int lfd = s_bind(AF_INET, SOCK_STREAM, 8000);
    if(lfd==-1){
        return -1;
    }
    //将socket设置为被动连接模式，当连接到来时，将连接放入未决连接队列中
    listen(lfd, 5);
    while(1){
        socklen_t cli_len = sizeof(cli);
        //从未决连接队列中取出第一个连接进行处理，返回一个连接描述符
        //将客户端的地址和端口号存放到cli指定的地址里
        cfd = accept(lfd, (SA *)&cli, &cli_len);
        if(cfd==-1){
            perror("accept");
            return -1;
        }
        //cli地址里存放的是客户端的ip地址和端口号，它们都是网络字节序的
        printf("client ip: %s\n", inet_ntop(AF_INET, &cli.sin_addr, IP, 64));
        //创建子进程
        pid_t pid = fork();
        if(pid==-1){
            perror("fork");
            return -1;
        }
        if(pid==0){//子进程执行的代码
            //关闭lfd
            close(lfd);
            //业务处理
            handle(cfd);
            //关闭本次连接
            close(cfd);
            exit(0);
        }
        else{//父进程执行的代码
            //关闭cfd
            close(cfd);
        }
    }
    close(lfd);
    return 0;
}