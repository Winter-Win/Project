#ifndef _TCP_CLIENT_HPP
#define _TCP_CLIENT_HPP

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

class TcpClient
{
private:
    int sock;
    string ip;
    int port;

public:
    TcpClient(string ip_, int port_):ip(ip_),port(port_)
    {
    }

    void InitClient()
    {
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if(sock < 0)
        {
            cerr << "socket error" << endl;
            exit(2);
        }
    }

    void Start()
    {
        struct sockaddr_in peer;
        bzero(&peer,sizeof(peer));
        peer.sin_family = AF_INET;
        peer.sin_port = htons(port);
        peer.sin_addr.s_addr = inet_addr(ip.c_str());

        if(connect(sock, (struct sockaddr*)&peer, sizeof(peer)) < 0)
        {
            cerr << "connect error" << endl;
            exit(3);
        }
        //char buf[1024];
        string in;
        while(1)
        {
            cout << "Please Enter# " << endl;
            cin >> in;
            write(sock, in.c_str(), in.size());
            char buf[1024];
            ssize_t s = read(sock, buf, sizeof(buf)-1);
            if(s > 0);
            {
                buf[s] = 0;
                cout << "Server Echo# " << buf << endl;
            }
        }
    }

    ~TcpClient()
    {
        if(sock > 0)
        {
            close(sock);
        }
    }

};

#endif
