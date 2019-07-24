#include "hello_http_server.hpp"

void Usage(string proc)
{
    cout << "Usage：" << proc << "ip port" << endl;
}

int main(int argc, char*  argv[])
{
    if(argc != 3)
    {
        Usage(argv[0]);
        exit(1);
    }
    int port = atoi(argv[2]);
    string ip = argv[1];
    TcpServer *tsp = new TcpServer(ip, port);
    tsp->InitServer();
    tsp->Start();
    return 0;
}


