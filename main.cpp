
#include "web_server.h"
#define SERVER_PORT 8001

int main(int argc,char* argv[]){
    web_server server;
    server.init(SERVER_PORT);
    server.event_listen();
    server.event_Loop();
    return 0;   
}