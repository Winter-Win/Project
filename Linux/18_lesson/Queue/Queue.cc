#include <iostream>
#include <queue>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

using namespace std;

class BlockQueue
{
private:
    queue<int> q;
    pthread_mutex_t lock;
    pthread_cond_t cond;

private:
    void LockQueue()
    {
        pthread_mutex_lock(&lock);
    }

    void UnlockQueue()
    {
        pthread_mutex_unlock(&lock);
    }

    bool isEmpty()
    {
        return q.size() == 0 ? true : false;
    }

    void ThreadWait()
    {
        pthread_cond_wait(&cond, &lock);
    }

    void WakeupOneThread()
    {
        pthread_cond_signal(&cond);
    }

public:
    BlockQueue()
    {
        pthread_mutex_init(&lock, NULL);
        pthread_cond_init(&cond, NULL);

    }

    void PushData(const int &data)
    {
        LockQueue();
        q.push(data);
        UnlockQueue();
        cout << "Product run done,data pop success : " << data << " wake up thread done..." << endl;
        WakeupOneThread();
    }

    void PopData(int &data)
    {
        LockQueue();
        while(isEmpty())
        {
            ThreadWait();
        }
        data = q.front();
        q.pop();
        UnlockQueue();
        cout << "Consume run done,data pop success : " << data << endl;
    }

    ~BlockQueue()
    {
        pthread_mutex_destroy(&lock);
        pthread_cond_destroy(&cond);

    }
};

void *product(void *arg)
{
    BlockQueue *bq = (BlockQueue*)arg;
    srand((unsigned long)time(NULL));
    while(1)
    {
        int data = rand() % 100 + 1;
        bq->PushData(data);
        sleep(1);
    }
}

void *consume(void  *arg)
{
    BlockQueue *bq = (BlockQueue*)arg;
    while(1)
    {
        int data;
        bq->PopData(data);
    }
}

int main()
{
    BlockQueue bq;
    pthread_t c, p;
    pthread_create(&c, NULL, consume, (void *)&bq);
    pthread_create(&p, NULL, product, (void *)&bq);

    pthread_join(c,NULL);
    pthread_join(p,NULL);

    return 0;
}
