#include"http_parser.h"


int http_parser::m_epollfd = -1;
int http_parser::m_user_count = 0;


void http_parser::init(int socket,sockaddr_in cliaddr){
    m_socekt = socket;
    address = cliaddr;
    memset(m_buf,'\0',BUFFER_SIZE);
    m_buf_len = 0;
    addfd(m_epollfd,m_socekt,EPOLLIN);
 }
bool http_parser::read_once()
{
    m_buf_len = read(m_socekt,m_buf,BUFFER_SIZE);
    if(m_buf_len==0){
        printf("客户端主动断开连接\n");
        //close(m_socekt);
        return 1;
    }
    return 0;
}


void http_parser::do_write(){
    
    write(1,m_buf,m_buf_len);
    write(m_socekt,m_buf,m_buf_len);
}
void http_parser::addfd(int epfd,int fd,uint32_t events){
    epoll_event event;
    event.events = events;
    event.data.fd = fd;
    epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&event);
}