#pragma once

#include <iostream>
#include <string>
#include "PlayerManager.hpp"

using namespace std;

class Hall
{
private:
    PlayerManager pm;

public:
    Hall()
    {
    }

    uint32_t Register(string &name, string &passwd) //注册方法
    {
        return pm.InsertPlayer(name, passwd);
    }

    uint32_t Login(uint32_t &id, string &passwd)
    {
        return pm.SearchPlayer(id, passwd);
    }

    ~Hall()
    {
    }
};

