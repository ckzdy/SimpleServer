#include "handle.h"

char work_dir[] = "/home/tarena/biaoc/web/html";

//从cfd获取客户端的请求消息，将第一行放入req中
static void set_req(int cfd, req_t *p_q){
    int r = 0;
    char cfd_buf[1024] = {0};
    char tmp_path[128] = {0};
    
    //获取客户端的请求消息
    r = read(cfd, cfd_buf, sizeof(cfd_buf));
    //write(1, cfd_buf, r);
    //解析cfd_buf的第一行，放入req中
    sscanf(cfd_buf, "%s %s %s\r\n",\
        p_q->method, p_q->path, p_q->proto);
    //如果请求时不指定资源，默认发送index.html
    if(strcmp(p_q->path,"/")==0){
        strcpy(p_q->path, "/index.html");
    }
    //设置请求报文中的资源的绝对路径
    strcpy(tmp_path, work_dir);
    strcat(tmp_path, p_q->path);
    strcpy(p_q->path, tmp_path);
    return;
}

//获取请求资源的类型
static void set_f_type(const char *path, char *f_type){
    if(strcmp(strrchr(path, '.'), ".html")==0)
        strcpy(f_type, "text/html");
    else if(strcmp(strrchr(path, '.'), ".jpg")==0)
        strcpy(f_type, "image/jpg");
    else if(strcmp(strrchr(path, '.'), ".png")==0)
        strcpy(f_type, "image/png");
    return;
}

//判断文件是否存在，设置响应报文的第一、第二行
static void set_res(const req_t *p_q, res_t *p_s){
    //设置响应报文中的状态码
    p_s->code = access(p_q->path, R_OK) ? 404 : 200;
    //设置响应报文中的第一行
    sprintf(p_s->fst_line, "%s %d\r\n", p_q->proto, p_s->code);
    //设置响应报文中的第二行（需要分别处理不同情况）
    if(p_s->code==404){
        strcpy(p_s->snd_line, "Content-Type: text/html\r\n\r\n");
    }
    else{
        sprintf(p_s->snd_line, "Content-Type: %s\r\n\r\n", p_s->f_type);
    }
    return;
}

//向客户端发送资源
static void respond(int cfd, const req_t *p_q, const res_t *p_s){
    //向浏览器写响应头
    write(cfd, p_s->fst_line, strlen(p_s->fst_line));
    write(cfd, p_s->snd_line, strlen(p_s->snd_line));
    //向浏览器传输文件
    //保存标准输出描述符的内容
    int sfd = dup(1);
    //将进程的标准输出重定向到浏览器
    dup2(cfd, 1);
    //关闭文件描述符
    close(cfd);
    //用cat将原本输出到显示器的内容输出到浏览器
    execlp("cat","cat", p_q->path, NULL);
    //恢复现场
    dup2(sfd, 1);
    return;
}

void handle(int cfd){
    req_t req = {0};
    res_t res = {0};
    //从cfd获取客户端的请求消息，将第一行放入req中
    set_req(cfd, &req);
    //获取请求资源的类型
    set_f_type(req.path, res.f_type);
    //printf("%s\t%s\t%s\n",req.method, req.path, \
        req.proto);
    
    //判断文件是否存在，设置响应报文的第一、第二行
    set_res(&req, &res);
    //printf("%s\t%s\n", res.fst_line, res.snd_line);
    //向客户端发送资源
    if(res.code==404){
        strncpy(req.path, work_dir, 128);
        strcat(req.path, "/lost.html");
        respond(cfd, &req, &res);
    }
    else{
        respond(cfd, &req, &res);
    }
    return;
}