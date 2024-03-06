#ifndef _LIST_TIMER_H
#define _LIST_TIMER_H
#include "timer.h"
//升序双向链表
class sort_timer_lst{
public:
    sort_timer_lst();
    ~sort_timer_lst();
    //正常的插入
    void add_timer(timer* timernode);

    void adjust_timer(timer* timernode);

    void del_timer(timer* timernode);
    
    //信号处理函数
    void tick();

private:
//从中间插入的重载
    void add_timer(timer* timernode,timer* lst_head);

private:
    timer* head;
    timer* tail;
};


#endif
