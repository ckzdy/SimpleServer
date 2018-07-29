#ifndef RESPOND_H
#define RESPOND_H
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>

typedef struct{
    char method[16];//请求报文中的方法
    char path[128];//请求报文中的资源的绝对路径
    char proto[16];//请求报文中的协议
}req_t;

typedef struct{
    int code;//响应报文中的状态码
    char fst_line[128];
    char snd_line[128];
    char f_type[32];//响应报文中的资源的类型
}res_t;

void handle(int);

#endif