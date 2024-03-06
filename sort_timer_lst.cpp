#include "sort_timer_lst.h"

sort_timer_lst::sort_timer_lst(){
    head = NULL;
    tail = NULL;
}

sort_timer_lst::~sort_timer_lst(){
    timer* tmp = head;
    while(tmp){
        head = tmp->next;
        delete tmp;
        tmp = head;
    }
}
    //正常的插入
void sort_timer_lst::add_timer(timer* timernode){
    if( !timernode )
    {
        return;
    }
    if( !head )
    {
        head = tail = timernode;
        return; 
    }
	/*如果目标定时器的超时时间小于当前链表中所有定时器的超时时间，
	则把该定时器插入链表的头部，作为链表的头节点；否则就需要调用重载函数
	add_timer(util_timer* timer, util_timer* lst_head)，把他插入链表中合适的位置；
	以保证链表的升序特性*/
    if( timernode->expire < head->expire )
    {
        timernode->next = head;
        head->prev = timernode;
        head = timernode;
        return;
    }
    add_timer( timernode, head );
}

void sort_timer_lst::add_timer( timer* timernode, timer* lst_head )
{
    timer* prev = lst_head;
    timer* tmp = prev->next;
	//遍历lst_head节点之后的部分链表，直到找到一个超时时间大于目标定时器的超时时间的节点，
	//并将目标定时器插入该节点之前
    while( tmp )
    {
        if( timernode->expire < tmp->expire )
        {
            prev->next = timernode;
            timernode->next = tmp;
            tmp->prev = timernode;
            timernode->prev = prev;
            break;
        }
        prev = tmp;
        tmp = tmp->next;
    }
	//如果遍历完lst_head节点之后的部分链表，仍未找到超时时间大于目标定时器的超时时间的节点，
	//则将目标定时器插入链表尾部，并把他设置为链表新的尾节点
    if( !tmp )
    {
        prev->next = timernode;
        timernode->prev = prev;
        timernode->next = NULL;
        tail = timernode;
    }
    
}
void sort_timer_lst::adjust_timer(timer* timernode){
    if( !timernode )
    {
        return;
    }
    timer* tmp = timernode->next;
    //如果被调整的目标定时器处在链表尾部，
	//或者该定时器新的超时值仍然小于其下一个定时器的超时值，则不调整
	if( !tmp || ( timernode->expire < tmp->expire ) )
    {
        return;
    }
	//如果目标定时器是链表的头节点，则将该定时器从链表中取出并重新插入链表
    if( timernode == head )
    {
        head = head->next;
        head->prev = NULL;
        timernode->next = NULL;
        add_timer( timernode, head );
    }
	//如果目标定时器不是链表的头节点，则将该定时器从链表中取出，
	//然后插入其原来所在位置之后的部分链表中
    else
    {
        timernode->prev->next = timernode->next;
        timernode->next->prev = timernode->prev;
        add_timer( timernode, timernode->next );
    }
}

void sort_timer_lst::del_timer(timer* timernode){
    if( !timernode )
    {
        return;
    }
	//下面这个条件成立表示链表中只有一个定时器，即目标定时器
    if( ( timernode == head ) && ( timernode == tail ) )
    {
        delete timernode;
        head = NULL;
        tail = NULL;
        return;
    }
	//如果链表中至少有两个定时器，且目标定时器是链表的头节点，
	//则将链表的头节点重置为原头节点的下一个节点，然后删除定时器
    if( timernode == head )
    {
        head = head->next;
        head->prev = NULL;
        delete timernode;
        return;
    }
	//如果链表中至少有两个定时器，且目标定时器是链表的尾节点，
	//则将链表的尾节点重置为原尾节点的前一个节点，然后删除定时器
    if( timernode == tail )
    {
        tail = tail->prev;
        tail->next = NULL;
        delete timernode;
        return;
    }
	//如果目标定时器位于链表的中间，则把它前后的定时器串联起来，然后删除目标定时器
    timernode->prev->next = timernode->next;
    timernode->next->prev = timernode->prev;
    delete timernode;
}
    
    //信号处理函数
void sort_timer_lst::tick(){
    if( !head )
    {
        printf( "无客户端\n" );
        return;
    }
    printf( "链表中有客户端\n" );
    //printf( "timer tick\n" );
    time_t cur = time( NULL );
    timer* tmp = head;
    while( tmp )
    {
		//因为每个定时器都使用绝对时间作为超时值，所以我们可以把
		//定时器的超时值和系统当前时间比较，以判断定时器是否到期
        if( cur < tmp->expire )
        {
            break;
        }
		//调用定时器的回调函数，以执行定时任务
        printf("存在客户端惰性连接，自动断开连接\n");
        tmp->cb_func( tmp->user_data );
		//执行完定时器中的定时任务之后，就将他从链表中删除，并重置链表头节点
        head = tmp->next;
        if( head )
        {
            head->prev = NULL;
        }
        delete tmp;
        tmp = head;
    }
}
