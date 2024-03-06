server:main.o http_parser.o web_server.o utils.o sort_timer_lst.o  
	g++ main.o http_parser.o web_server.o utils.o sort_timer_lst.o -o server
main.o:main.cpp
	g++ -c main.cpp -o main.o
http_parser.o:http_parser.cpp
	g++ -c http_parser.cpp -o http_parser.o
web_server.o:web_server.cpp
	g++ -c web_server.cpp -o web_server.o
utils.o:utils.cpp
	g++ -c utils.cpp -o utils.o
sort_timer_lst.o:sort_timer_lst.cpp
	g++ -c sort_timer_lst.cpp -o sort_timer_lst.o
.PHONY:clean
clean:
	rm -rf main.o http_parser.o web_server.o sort_timer_lst.o  utils.o server