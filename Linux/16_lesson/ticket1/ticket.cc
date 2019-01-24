#include <iostream>
#include <pthread.h>
using namespace std;

int tickets = 1001;

void *Buyticket(void *arg)
{
    while(1)
    {
        if(tickets > 0)
        {
            cout << "Get ticket success : " << tickets << endl;
            tickets--;
        }
        else
        {
            break;
        }
    }
}

int main()
{
    pthread_t tid[5];
    for(int i = 0; i < 5; i++)
    {
        pthread_create(tid+i, NULL, Buyticket, NULL);
    }

    for(int i = 0; i < 5; i++)
    {
        pthread_join(tid[i], NULL);
    }
    return 0;
}
