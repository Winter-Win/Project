.PHONY:all
all:http_server

http_server:hello_http_server.cc
	g++ -o $@ $^

.PHONY:clean
clean:
	rm -f http_server
