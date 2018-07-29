#ifndef T_NET_H
#define T_NET_H
/*其他头文件*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <dirent.h>

/*类型的声明*/
typedef struct sockaddr SA;
typedef struct sockaddr_in SA4;

/*函数的声明*/
int s_bind(int, int, unsigned short);
#endif