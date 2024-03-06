#include "utils.h"

int *utils::u_pipefd = 0;
int utils::u_epollfd = -1;
void utils::init(int timeslot){
    m_TIMESLOT = timeslot;
}

// void utils::addsig(int sig,void(handler)(int),bool restart = true){
void utils::addsig(int sig,void(handler)(int),bool restart){
    struct sigaction sa;
    sa.sa_handler = handler;
    if(restart){

        sa.sa_flags |= SA_RESTART;//设置重启系统调用

    }else{
        sa.sa_flags = 0;
    }
    //全部填充
    sigfillset(&sa.sa_mask);
    sigaction(sig,&sa,NULL);

}


void utils::sig_handler(int sig){
    //为了保证函数的课重入性，保留原来的errno
    int save_errno = errno;
    int msg = sig;
    write(1,(char *)&msg,1);
    write(u_pipefd[1],(char *)&msg,1);
    errno = save_errno;
}

void utils::timer_handle(){
    m_timer_lst.tick();
    alarm(m_TIMESLOT);
}

class utils;
void cb_func(http_parser* user_data){
    epoll_ctl(utils::u_epollfd,EPOLL_CTL_DEL,user_data->m_socekt,NULL);
    close(user_data->m_socekt);
}