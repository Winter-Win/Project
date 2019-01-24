#include <iostream>
#include <pthread.h>
using namespace std;

int tickets = 1001;
pthread_mutex_t mutex;

void *Buyticket(void *arg)
{
    while(1)
    {
        pthread_mutex_lock(&mutex);
        if(tickets > 0)
        {
            cout << "Get ticket success : " << tickets << endl;
            tickets--;
            pthread_mutex_unlock(&mutex);
        }
        else
        {
            pthread_mutex_unlock(&mutex);
            break;
        }
    }
}

int main()
{
    pthread_mutex_init(&mutex, NULL);
    pthread_t tid[5];
    for(int i = 0; i < 5; i++)
    {
        pthread_create(tid+i, NULL, Buyticket, NULL);
    }

    for(int i = 0; i < 5; i++)
    {
        pthread_join(tid[i], NULL);
    }
    pthread_mutex_destroy(&mutex);
    return 0;
}
