#ifndef _TIMER_H
#define _TIMER_H

#include <time.h>
#include "http_parser.h"

class http_parser;

class timer{

public:
    timer():prev(NULL),next(NULL){}
public:
    time_t expire;
    void(*cb_func)(http_parser*);
    http_parser* user_data;
    timer* prev;
    timer* next;
};



#endif