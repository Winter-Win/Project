#include <iostream>
using namespace std;

int main(int argc, char *argv[], char *env[])
{
    int i = 0;
    for(i = 0 ; env[i]; i++)
    {
       cout << env[i] << endl;
    }
    return 0;
}
