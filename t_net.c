#include "t_net.h"

int s_bind(int domain, int type, unsigned short port){
    SA4 serv;
    //创建一个socket（套接字），返回该socket的文件描述符（lfd）
    int lfd = socket(domain, type, 0);
    if(lfd==-1){
        perror("socket");
        return -1;
    }
    //对serv变量的成员进行初始化
    serv.sin_family = AF_INET;
    serv.sin_port = htons(port);
    serv.sin_addr.s_addr = htonl(INADDR_ANY);
    //将serv绑定到socket上
    int b = bind(lfd, (SA *)&serv, sizeof(serv));
    if(b==-1){
        perror("bind");
        return -1;
    }
    return lfd;
}