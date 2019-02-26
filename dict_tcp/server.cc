#include "server.hpp"

//直接不用在服务器端输入ip地址，有关ip的都可以去掉，但是客户端必须有服务器的ip地址才能连接上来
void Usage(string proc)
{
    cout << "Usage：" << proc << "port" << endl;
}

int main(int argc, char*  argv[])
{
    if(argc != 2)
    {
        Usage(argv[0]);
        exit(1);
    }
    
    dict.insert(pair<string, string>("apple", "苹果"));
    dict.insert(pair<string, string>("banana", "香蕉"));
    dict.insert(pair<string, string>("peach", "桃子"));
    dict.insert(pair<string, string>("lemon", "柠檬"));

    int port = atoi(argv[1]);
    TcpServer *tsp = new TcpServer(port);
    tsp->InitServer();
    tsp->Start();
    return 0;
}


