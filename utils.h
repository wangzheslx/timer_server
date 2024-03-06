#ifndef _UTILS_H
#define _UTILS_H
#include "sort_timer_lst.h"
#include <signal.h>
#include <errno.h>
class utils{
public:
    utils(){}
    ~utils(){}
    void init(int timeslot);

    //void addfd(int epollfd,int fd,bool one_shot,int TRIGMode);
    //void addfd(int epollfd,int fd);
    //传的函数指针必须用static
    static void sig_handler(int sig);
    
    void addsig(int sig,void(handler)(int),bool restart = true);
    
    void timer_handle();
    
    //void show_error(int connfd,const char * info);
public:
    //描述管道读写段的描述符
    static int *u_pipefd;
    //链表
    sort_timer_lst m_timer_lst;
    static int u_epollfd;
    //时长
    int m_TIMESLOT;
};

void cb_func(http_parser* user_data);

#endif