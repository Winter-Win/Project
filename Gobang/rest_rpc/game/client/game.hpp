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
		    return 3;
    	}
        id = client.call<uint32_t>("RpcRegister", name, passwd);
    }
    catch (const std::exception& e)
    {
		std::cout << e.what() << std::endl;
	}
    return id;
}

uint32_t Login(const string &ip, int &port)
{
    uint32_t id;
    string passwd;

    cout << "请输入你的登录ID: ";
    cin >> id;
    cout << "请输入你的登录密码: ";
    cin >> passwd;

    uint32_t result = 0;
	try
    {
		rpc_client client(ip, port);
    	bool r = client.connect();
    	if (!r)
        {
		    cout << "connect timeout" << endl;
		    return 3;
    	}
        result = client.call<uint32_t>("RpcLogin", id, passwd);
    }
    catch (const std::exception& e)
    {
		std::cout << e.what() << std::endl;
    }

    return result;
}

bool Match(uint32_t &id, const string &ip, uint32_t &port)
{
	try
    {
		rpc_client client(ip, port);
    	bool r = client.connect();
    	if (!r)
        {
		    cout << "connect timeout" << endl;
		    return 3;
    	}
        return client.call<bool>("RpcMatchAndWait", id);
    }
    catch (const std::exception& e)
    {
		std::cout << e.what() << std::endl;
    }
}

void PlayGame()
{

}

void Game(uint32_t &id)
{
    int select = 0;
    volatile bool quit = false;
    while(!quit)
    {
        cout << "#########################################" << endl;
        cout << "####    1. 匹配           2. 退出    ####" << endl;
        cout << "#########################################" << endl;
        cout << "请选择: ";
        cin >> select;
        switch(select)
        {
            case 1:
                {
                    if(Match(id)) //匹配
                    {
                        PlayGame();
                    }
                    else
                    {
                        cout << "匹配失败，请重试！" << endl;
                    }
                }
                break;
            case 2:
                quit = true;
                break;
            default:
                cout << "选择错误，请重新输入！" << endl;
                break;
        }
    }
}
