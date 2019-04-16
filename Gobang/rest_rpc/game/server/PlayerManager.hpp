#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>   //  std::pair

#include "Log.hpp"
#include "Player.hpp"


#define DEFAULT_ID 10000

using namespace std;

class PlayerManager
{
private:
    unordered_map<uint32_t, Player> player_set;
    uint32_t assign_id;

public:
    PlayerManager():assign_id(DEFAULT_ID)
    {
    }

    //插入用户以及注册出来了
    uint32_t InsertPlayer(string &name, string &passwd)
    {
        uint32_t id_ = assign_id++;
        Player p(name, passwd, id_);
        //player_set.insert(make_pair<uint32_t, Player>(id_, p));
        player_set.insert({id_, p});
        LOG(INFO, "玩家正在注册插入中...");
        return id_;
    }

    //查找，负责在登录时用
    uint32_t SearchPlayer(uint32_t &id, string& passwd)
    {
        //1、迭代器
        //2、find
        auto iter = player_set.find(id);
        if(iter == player_set.end())
        {
            LOG(WARNING, "查找拥护呼失败...");
            return 1;
        }

        Player &p = iter->second; //拿到用户的第二个信息
        if(p.Passwd() != passwd)
        {
            LOG(WARNING, "密码错误...");
            return 2;
        }
        p.Online(); //置为在线
        LOG(INFO, "用户登录成功...");
        return id;
    }

    ~PlayerManager()
    {
    }
};
