#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
    extern char **environ;
    int i = 0;
    for(i = 0; environ[i]; i++)
    {
        cout << environ[i] << endl;
    }
    return 0;
}
