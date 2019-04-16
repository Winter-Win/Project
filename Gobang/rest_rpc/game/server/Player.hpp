#pragma once

#include <iostream>
#include <string>

using namespace std;

typedef enum
{
    OFFLINE,
    ONLINE,
    MATCHING,
    PLAYING,
}status_t;


class Player
{
private: //相当于数据库，以及表
    string name;
    string passwd;
    uint32_t id; //

    int win;
    int lose;
    int tie; //平局

    status_t st;

public:
    Player()
    {
    }

    Player(string &name_, string &passwd_, uint32_t &id_)
        :name(name_),passwd(passwd_),id(id_)
    {
        win = 0;
        lose = 0;
        tie = 0;
        st = OFFLINE;
    }

    const string& Passwd()
    {
        return passwd;
    }

    void Online()
    {
        st = ONLINE;
    }

    int Rate() //胜率
    {
        int total = win + lose;
        if(total == 0)
        {
            return 0;
        }
        return win*100/total;
    }

    ~Player()
    {
    }


};
