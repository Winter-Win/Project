#include <iostream>
#include <pthread.h>
#include <unistd.h>
using namespace std;

pthread_mutex_t lock;
pthread_cond_t cond;

void *thread_run(void *arg)
{
    while(1)
    {
        pthread_cond_wait(&cond, &lock);
        cout << "active..." << endl;
    }
}

void *thread_signal(void *arg)
{
    while(1)
    {
        sleep(1);
        pthread_cond_signal(&cond);
    }
}

int main()
{
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_t tid1, tid2;
    pthread_create(&tid1, NULL, thread_run, NULL);
    pthread_create(&tid2, NULL, thread_signal, NULL);


    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);

    return 0;
}
