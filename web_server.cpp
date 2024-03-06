#include"web_server.h"

web_server::web_server(){
    users = new http_parser[MAX_FD];
}

web_server::~web_server(){
}

void web_server::init(int port){
    m_listenfd = -1;
    m_port = port;
    m_epollfd = -1;
    memset(events,'\0',sizeof(events));
}
void web_server::event_listen(){
    m_listenfd = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in address;
    bzero(&address,sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(m_port);
    address.sin_addr.s_addr = INADDR_ANY;
    int flag = 1;
    //setsockopt(m_listenfd,SOL_SOCKET,SO_REUSEADDR,&flag,sizeof(flag));
    int ret = bind(m_listenfd,(struct  sockaddr*)&address,sizeof(address));
    if(ret == -1){
        perror("bind error");
        exit(1);
    }
    listen(m_listenfd,64);
    m_epollfd = epoll_create(64);
    http_parser::m_epollfd = m_epollfd;
    //初始化定时器
    utils::u_epollfd = m_epollfd;
    addfd(m_epollfd,m_listenfd,EPOLLIN);
    //定时器
    pipe(m_pipefd);
    utils::u_pipefd = m_pipefd;
    addfd(m_epollfd,m_pipefd[0],EPOLLIN);
    util.init(5);
    util.addsig(SIGALRM,util.sig_handler);
    alarm(util.m_TIMESLOT);
    

}
void web_server::event_Loop(){
    bool timeout = false;
    while(1){
        int number = epoll_wait(m_epollfd,events,MAX_EVENT_NUMBER,-1);
        for(int i = 0;i<number;i++){
            int sockfd = events[i].data.fd;
            if(sockfd == m_listenfd){
                deal_new_connection();
            }else if(m_pipefd[0] == sockfd && events[i].events &EPOLLIN)
            {
                deal_signal_event(timeout);
            }
            else if(events[i].events&EPOLLIN){
                deal_read_event(sockfd);
                update_signal(sockfd);

            }
            if(timeout == true){
                printf("检测链表中是否超时\n");
                util.timer_handle();
                timeout = false;
            }
        }

    }
}
void web_server::deal_new_connection(){
    struct sockaddr_in client_address;
    socklen_t client_addrlength = sizeof(client_address);
    int connfd = accept(m_listenfd,(sockaddr*)&client_address,&client_addrlength);
    if(connfd<0){
        perror("accept error");
        exit(1);
    }
    if(http_parser::m_user_count >= MAX_FD){
        perror("outloop");
        exit(1);
    }
    printf("有新用户建立连接\n");
    newtimer(connfd,client_address);
}
void web_server::deal_read_event(int socket){
    bool flag = 0;
    flag = users[socket].read_once();
    if(flag){
        //删除定时器链表当中的客户端
        util.m_timer_lst.del_timer(users[socket].m_timer);
        //关闭文件描述符
        close(socket);
        //从监听链表中移除
        epoll_ctl(m_epollfd,EPOLL_CTL_DEL,socket,NULL);
        return ;
    }else{
        users[socket].do_write();
    }
    

}
void web_server::addfd(int epfd,int fd,uint32_t events){
    epoll_event event;
    event.events = events;
    event.data.fd = fd;
    epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&event);
}

//给新的客户端分配一个定时器
void web_server::newtimer(int connfd,sockaddr_in client_address){
    users[connfd].init(connfd,client_address);
    timer* timernode = new timer;
    timernode->user_data = &users[connfd];
    timernode->cb_func = cb_func;
    time_t cur = time(NULL);
    timernode->expire = cur + TIMESLOT *3;
    util.m_timer_lst.add_timer(timernode);
    users[connfd].m_timer = timernode;

}

void web_server::deal_signal_event(bool &timeout){
    int ret;
    char signal[1024];
    ret = read(m_pipefd[0],signal,sizeof(signal));
    if(signal[0] == SIGALRM){
        timeout = true;
    }
}

//当客户端活跃时更新链表时间
void web_server::update_signal(int socket){
    time_t cur = time(NULL);
    users[socket].m_timer->expire = cur+TIMESLOT*3;
    util.m_timer_lst.adjust_timer(users[socket].m_timer);
}