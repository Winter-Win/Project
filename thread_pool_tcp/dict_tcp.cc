#include <iostream>
#include <map>

using namespace std;

int main()
{
    map<string, string> dict;
    dict.insert(pair<string, string>("apple", "苹果"));
    dict.insert(pair<string, string>("banana", "香蕉"));
    dict.insert(pair<string, string>("peach", "桃子"));
    dict.insert(pair<string, string>("lemon", "柠檬"));

    for(auto iter = dict.begin(); iter != dict.end(); iter++)
    {
        cout << iter->first << ":" << iter->second << endl;
    }

    cout << dict["banana"] << endl;

    auto it = dict.find("lemon");
    cout << it->first << " : " << it->second << endl;
}
