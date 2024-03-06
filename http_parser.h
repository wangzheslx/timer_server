#ifndef _HTTP_PASER_H
#define _HTTP_PASER_H
#include<stdio.h>
#include<netinet/ip.h>
#include<netinet/in.h>
#include<sys/epoll.h>
#include<string.h>
#include<error.h>
#include<stdlib.h>
#include <unistd.h>
#include "timer.h"

class timer;

class http_parser{
public:
    static const int BUFFER_SIZE = 1024;


public:

    void init(int socket,sockaddr_in cliaddr);
    bool read_once();
    void do_write();
    void addfd(int epfd,int fd,uint32_t events);
    int m_socekt;
    int m_buf_len;
    sockaddr_in address;
    char m_buf[BUFFER_SIZE];
    static int m_epollfd;
    static int m_user_count;

    timer* m_timer;
};


#endif