#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

using namespace std;

int data = 0;
pthread_rwlock_t rw;

void *reader(void *arg)
{
    while(1)
    {
        pthread_rwlock_rdlock(&rw);
        cout << "Reader read done : " << data << endl;
        pthread_rwlock_unlock(&rw);
        usleep(90);
    }
}

void *writer(void *arg)
{
    while(1)
    {
        pthread_rwlock_wrlock(&rw);
        cout <<  "Writer write success : " << data << endl;
        data++;
        pthread_rwlock_unlock(&rw);
        usleep(90);
    }
}

int main()
{
    pthread_rwlock_init(&rw, NULL);
    pthread_t r[5], w;

    pthread_create(&r[1], NULL, reader, NULL);
    pthread_create(&r[2], NULL, reader, NULL);
    pthread_create(&r[3], NULL, reader, NULL);
    pthread_create(&r[4], NULL, reader, NULL);
    pthread_create(&r[5], NULL, reader, NULL);
    pthread_create(&w, NULL, writer, NULL);

    pthread_join(r[1], NULL);
    pthread_join(r[2], NULL);
    pthread_join(r[3], NULL);
    pthread_join(r[4], NULL);
    pthread_join(r[5], NULL);
    pthread_join(w, NULL);
    
    pthread_rwlock_destroy(&rw);
    return 0;
}
