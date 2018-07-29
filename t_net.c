#include "t_net.h"

int s_bind(int domain, int type, unsigned short port){
    SA4 serv;
    //����һ��socket���׽��֣������ظ�socket���ļ���������lfd��
    int lfd = socket(domain, type, 0);
    if(lfd==-1){
        perror("socket");
        return -1;
    }
    //��serv�����ĳ�Ա���г�ʼ��
    serv.sin_family = AF_INET;
    serv.sin_port = htons(port);
    serv.sin_addr.s_addr = htonl(INADDR_ANY);
    //��serv�󶨵�socket��
    int b = bind(lfd, (SA *)&serv, sizeof(serv));
    if(b==-1){
        perror("bind");
        return -1;
    }
    return lfd;
}