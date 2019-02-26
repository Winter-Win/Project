#ifndef _TCP_SERVER_HPP
#define _TCP_SERVER_HPP

#include <iostream>
#include <string>
#include <map>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

class TcpServer;

map<string, string> dict;

class Arg
{
public:
    string ip;
    int port;
    int sock;
    TcpServer *tsp;
};

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

                //读取字典中的信息
                auto it = dict.find(buf);
                if(it == dict.end())
                {
                    write(sock, "NULL", strlen("NULL"));
                    continue;
                }
                write(sock, (it->second).c_str(), (it->second).size());
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

    static void *HandlerRequest(void *arg)
    {
        pthread_detach(pthread_self());
        Arg *sarg = (Arg*)arg;
        sarg->tsp->Service(sarg->sock, sarg->ip,sarg->port);
        delete sarg;
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

            //多线程
            pthread_t tid;
            Arg *arg = new Arg;
            arg->sock = sock;
            arg->ip = ip_;
            arg->port = port_;
            arg->tsp = this;
            pthread_create(&tid, NULL, HandlerRequest, (void*)arg);//到时候直接线程分离，不用join
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
