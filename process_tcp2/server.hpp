#ifndef _TCP_SERVER_HPP
#define _TCP_SERVER_HPP

#include <iostream>
#include <string>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

class TcpServer
{
private:
    int listen_sock;
    int port;

public:
    TcpServer(int port_):port(port_),listen_sock(-1)
    {
    }

    void InitServer()
    {
        listen_sock = socket(AF_INET, SOCK_STREAM, 0);
        if(listen_sock < 0)
        {
            cerr << "socket error" << endl;
            exit(2);
        }
        
        //服务器挂掉之后可以直接重启
        int opt = 1;
        setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        struct sockaddr_in local;
        bzero(&local,sizeof(local));
        local.sin_family = AF_INET;
        local.sin_port = htons(port);
        //local.sin_addr.s_addr = inet_addr(ip.c_str());
        local.sin_addr.s_addr = htonl(INADDR_ANY);
        if(bind(listen_sock, (struct sockaddr*)&local, sizeof(local)) < 0)
        {
            cerr << "bind error" << endl;
            exit(3);
        }
        if(listen(listen_sock, 5) < 0)
        {
            cout << "listen error" << endl;
            exit(4);
        }
    }

    void Service(int sock, string ip_, int port_)
    {
        while(1)
        {
            char buf[1024];
            ssize_t s = read(sock, buf, sizeof(buf)-1);
            if(s > 0)
            {
                buf[s] = 0;
                cout << "[ " << ip_ << ":" << port_ << " ]# " << buf << endl;
                write(sock, buf, strlen(buf));
            }
            else if(s == 0)
            {
                cout << "client quit!" << endl;
                break;
            }
            else
            {
                cerr << "read error" << endl;
                break;
            }
        }
        close(sock);
    }

    void Start()
    {
        struct sockaddr_in peer;
        socklen_t len = 0;
        while(1)
        {
            len = sizeof(peer);
            int sock = accept(listen_sock, (struct sockaddr*)&peer, &len);
            if(sock < 0)
            {
                cerr << "accept error!" << endl;
                continue;
            }
            cout << "Get a new client! " << sock << endl;
            //将客户端的地址转换出来
            int port_ = ntohs(peer.sin_port);
            string ip_ = inet_ntoa(peer.sin_addr);

            //子进程执行，父进程忽略，各自关闭没必要的文件描述符
            pid_t id = fork();
            if(id < 0)
            {
                cerr << "fork error" << endl;
                close(sock);
                continue;
            }
            else if(id == 0)
            {//child
                close(listen_sock);
                Service(sock, ip_, port_);
                exit(0);
            }
            else
            {//father
                //wait?
                //not do
                close(sock);
            }
        }
    }

    ~TcpServer()
    {
        if(listen_sock > 0)
        {
            close(listen_sock);
        }
    }
};

#endif
