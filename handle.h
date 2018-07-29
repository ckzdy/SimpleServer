#ifndef RESPOND_H
#define RESPOND_H
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>

typedef struct{
    char method[16];//�������еķ���
    char path[128];//�������е���Դ�ľ���·��
    char proto[16];//�������е�Э��
}req_t;

typedef struct{
    int code;//��Ӧ�����е�״̬��
    char fst_line[128];
    char snd_line[128];
    char f_type[32];//��Ӧ�����е���Դ������
}res_t;

void handle(int);

#endif