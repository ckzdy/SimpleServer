#ifndef T_NET_H
#define T_NET_H
/*����ͷ�ļ�*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <dirent.h>

/*���͵�����*/
typedef struct sockaddr SA;
typedef struct sockaddr_in SA4;

/*����������*/
int s_bind(int, int, unsigned short);
#endif