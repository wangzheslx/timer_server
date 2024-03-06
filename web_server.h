#ifndef _WEB_SERVER_H
#define _WEB_SERVER_H

#include "http_parser.h"
#include "utils.h"
#define MAX_EVENT_NUMBER 2000 //监听时间的最大个数
#define MAX_FD 2000 //文件描述符上限 

class utils;

const int TIMESLOT = 5;
class web_server{

public:
    
    web_server();
    ~web_server();
    void init(int port);
    void event_listen();
    void event_Loop();
    void deal_new_connection();
    void deal_read_event(int socket);
    void addfd(int epfd,int fd,uint32_t events);

    void newtimer(int connfd,sockaddr_in client_address);
    void deal_signal_event(bool &timeout);
    void update_signal(int socket);
public:
    int m_listenfd;
    int m_port;
    int m_epollfd;
 //   char* m_root;
    http_parser* users;
    epoll_event events[MAX_EVENT_NUMBER];


    int m_pipefd[2];
    utils util;
};


#endif