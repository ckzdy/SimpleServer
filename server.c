#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
#include "t_net.h"
#include "handle.h"
//�������յ����źţ������ӽ��̵���Դ
void doit(int n){
    waitpid(-1, NULL, WNOHANG);
    return;
}
int main(){
    int cfd;//���ںͿͻ���ͨѶ��������
    char IP[64];
    //����һ��ipv4��ַ�������͵ı��������Ǿ����ͨ�ŵ�ַ������������ip��ַ�Ͷ˿ں�
    SA4 cli;
    signal(SIGCHLD, doit);
    //����s_bind����
    int lfd = s_bind(AF_INET, SOCK_STREAM, 8000);
    if(lfd==-1){
        return -1;
    }
    //��socket����Ϊ��������ģʽ�������ӵ���ʱ�������ӷ���δ�����Ӷ�����
    listen(lfd, 5);
    while(1){
        socklen_t cli_len = sizeof(cli);
        //��δ�����Ӷ�����ȡ����һ�����ӽ��д�������һ������������
        //���ͻ��˵ĵ�ַ�Ͷ˿ںŴ�ŵ�cliָ���ĵ�ַ��
        cfd = accept(lfd, (SA *)&cli, &cli_len);
        if(cfd==-1){
            perror("accept");
            return -1;
        }
        //cli��ַ���ŵ��ǿͻ��˵�ip��ַ�Ͷ˿ںţ����Ƕ��������ֽ����
        printf("client ip: %s\n", inet_ntop(AF_INET, &cli.sin_addr, IP, 64));
        //�����ӽ���
        pid_t pid = fork();
        if(pid==-1){
            perror("fork");
            return -1;
        }
        if(pid==0){//�ӽ���ִ�еĴ���
            //�ر�lfd
            close(lfd);
            //ҵ����
            handle(cfd);
            //�رձ�������
            close(cfd);
            exit(0);
        }
        else{//������ִ�еĴ���
            //�ر�cfd
            close(cfd);
        }
    }
    close(lfd);
    return 0;
}