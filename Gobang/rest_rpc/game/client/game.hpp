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

bool PushMatchPool(uint32_t &id, const string &ip, int &port)
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

int CheckReady(uint32_t &id, const string &ip, int &port)
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
        return client.call<int>("RpcPlayerReady", id);
    }
    catch (const std::exception& e)
    {
		std::cout << e.what() << std::endl;
    }
}

bool Match(uint32_t &id, const string &ip, int port)
{
    PushMatchPool(id, ip, port);
    while(1)
    {
        int result = CheckReady(id, ip, port);
        cout << "check: " << result << endl;
        if(result == 3)
        {
            return true;
        }
        else if(result == 1)
        {
            return false;
        }
        else
        {
            sleep(1);
        }
    }
}

int GetBoard(uint32_t &room_id, const string &ip, int &port, string &board)
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
        board =  client.call<string>("RpcBoard", room_id);
    }
    catch (const std::exception& e)
    {
		std::cout << e.what() << std::endl;
    }
}

uint32_t GetMyRoomId(uint32_t &id, const string &ip, int &port)
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
        return client.call<uint32_t>("RpcPlayerRoomId", id);
    }
    catch (const std::exception& e)
    {
		std::cout << e.what() << std::endl;
    }

}

char GetMyPiece(uint32_t &room_id, uint32_t &id, const string &ip, int &port)
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
        return client.call<char>("RpcPlayerPiece",room_id);
    }
    catch (const std::exception& e)
    {
		std::cout << e.what() << std::endl;
    }
}

void ShowBoard(string &board)
{
    for(auto i = 0; i < board.size(); ++i)
    {
        cout << '|' << board[i] << '|' << endl;
    }
}

bool IsMyTurn()
{
    return false;
}

bool PosIsRight(int x, int y)
{
    return false;
}

void Step(uint32_t &id, int x, int y)
{

}

char Judge(uint32_t id)
{
    return 'N';
}

void PlayGame(uint32_t &id, const string &ip, int &port)
{
    cout << "匹配成功，请开始游戏..." << endl;
    int x, y;
    char result = 'N';
    string board;
    uint32_t room_id = GetMyRoomId(id, ip, port);
    cout << "room_id: " << room_id << endl;
    char piece = GetMyPiece(room_id, id, ip, port);
    cout << "piece: " << piece << endl;
    while(1)
    {
        GetBoard(room_id, ip, port, board);
        ShowBoard(board);
        if(!IsMyTurn())
        {
            sleep(1);
            continue;
        }
        cout << "请输入你的落子位置:>" ;
        cin >> x >> y;
        if(x >= 1 && x <= 5 && y >= 1 && y <= 5)
        {
            if(!PosIsRight(x, y))
            {
                cout << "你输入的位置已经被占用，请重新输入！" << endl;
            }
            else
            {
                Step(id, x, y);
                result = Judge(id);
                if(result != 'N')
                {
                    break;
                }
            }
        }
        else
        {
            cout << "你输入的位置有误，请重新输入！" << endl;
        }

        switch(result)
        {
            case 'X':
                break;
            case 'O':
                break;
            case 'E':
                cout << "平局，恭喜..." << endl;
                break;
            default:
                break;
        }
    }
}

void Game(uint32_t &id, const string &ip, int &port)
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
                    if(Match(id, ip, port)) //匹配
                    {
                        PlayGame(id, ip, port);
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
