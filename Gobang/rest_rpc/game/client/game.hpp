#pragma once

#include <iostream>
#include <rpc_client.hpp>
#include <chrono>
#include <fstream>
#include "codec.h"
using namespace std;
using namespace std::chrono_literals;
using namespace rest_rpc;
using namespace rest_rpc::rpc_service;


uint32_t Register(const string &ip, const int &port, string &name, string &passwd, uint32_t &id)
{
    cout << "请输入的昵称: ";
    cin >> name;
    cout << "请输入密码: ";
    cin >> passwd;
    string passwd_;
    cout << "请再次输入密码: ";
    cin >> passwd_;
    if(passwd != passwd_)
    {
        cout << "密码不一致！" << endl;
        return 1;
    }

	try
    {
		rpc_client client(ip, port);
    	bool r = client.connect();
    	if (!r)
        {
		    cout << "connect timeout" << endl;
		    return 2;
    	}
        id = client.call<uint32_t>("RpcRegister", name, passwd);
    }
    catch (const std::exception& e)
    {
		std::cout << e.what() << std::endl;
	}
    return id;
}

int Login()
{
    return 0;
}
