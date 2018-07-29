#include "handle.h"

char work_dir[] = "/home/tarena/biaoc/web/html";

//��cfd��ȡ�ͻ��˵�������Ϣ������һ�з���req��
static void set_req(int cfd, req_t *p_q){
    int r = 0;
    char cfd_buf[1024] = {0};
    char tmp_path[128] = {0};
    
    //��ȡ�ͻ��˵�������Ϣ
    r = read(cfd, cfd_buf, sizeof(cfd_buf));
    //write(1, cfd_buf, r);
    //����cfd_buf�ĵ�һ�У�����req��
    sscanf(cfd_buf, "%s %s %s\r\n",\
        p_q->method, p_q->path, p_q->proto);
    //�������ʱ��ָ����Դ��Ĭ�Ϸ���index.html
    if(strcmp(p_q->path,"/")==0){
        strcpy(p_q->path, "/index.html");
    }
    //�����������е���Դ�ľ���·��
    strcpy(tmp_path, work_dir);
    strcat(tmp_path, p_q->path);
    strcpy(p_q->path, tmp_path);
    return;
}

//��ȡ������Դ������
static void set_f_type(const char *path, char *f_type){
    if(strcmp(strrchr(path, '.'), ".html")==0)
        strcpy(f_type, "text/html");
    else if(strcmp(strrchr(path, '.'), ".jpg")==0)
        strcpy(f_type, "image/jpg");
    else if(strcmp(strrchr(path, '.'), ".png")==0)
        strcpy(f_type, "image/png");
    return;
}

//�ж��ļ��Ƿ���ڣ�������Ӧ���ĵĵ�һ���ڶ���
static void set_res(const req_t *p_q, res_t *p_s){
    //������Ӧ�����е�״̬��
    p_s->code = access(p_q->path, R_OK) ? 404 : 200;
    //������Ӧ�����еĵ�һ��
    sprintf(p_s->fst_line, "%s %d\r\n", p_q->proto, p_s->code);
    //������Ӧ�����еĵڶ��У���Ҫ�ֱ���ͬ�����
    if(p_s->code==404){
        strcpy(p_s->snd_line, "Content-Type: text/html\r\n\r\n");
    }
    else{
        sprintf(p_s->snd_line, "Content-Type: %s\r\n\r\n", p_s->f_type);
    }
    return;
}

//��ͻ��˷�����Դ
static void respond(int cfd, const req_t *p_q, const res_t *p_s){
    //�������д��Ӧͷ
    write(cfd, p_s->fst_line, strlen(p_s->fst_line));
    write(cfd, p_s->snd_line, strlen(p_s->snd_line));
    //������������ļ�
    //�����׼���������������
    int sfd = dup(1);
    //�����̵ı�׼����ض��������
    dup2(cfd, 1);
    //�ر��ļ�������
    close(cfd);
    //��cat��ԭ���������ʾ������������������
    execlp("cat","cat", p_q->path, NULL);
    //�ָ��ֳ�
    dup2(sfd, 1);
    return;
}

void handle(int cfd){
    req_t req = {0};
    res_t res = {0};
    //��cfd��ȡ�ͻ��˵�������Ϣ������һ�з���req��
    set_req(cfd, &req);
    //��ȡ������Դ������
    set_f_type(req.path, res.f_type);
    //printf("%s\t%s\t%s\n",req.method, req.path, \
        req.proto);
    
    //�ж��ļ��Ƿ���ڣ�������Ӧ���ĵĵ�һ���ڶ���
    set_res(&req, &res);
    //printf("%s\t%s\n", res.fst_line, res.snd_line);
    //��ͻ��˷�����Դ
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