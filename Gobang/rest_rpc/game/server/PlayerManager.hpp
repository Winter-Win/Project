#pragma once

#include <iostream>
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
        cout << "玩家正在注册插入中..." << endl;
        return id_;
    }

    ~PlayerManager()
    {
    }
};
