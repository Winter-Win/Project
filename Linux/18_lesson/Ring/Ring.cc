#include <iostream>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

using namespace std;

#define NUM 16

class Ring
{
private:
   int ring[NUM];
   int p_step;
   int c_step;
   sem_t blank_sem;
   sem_t data_sem;
public:
   void P(sem_t *sem)
   {
       sem_wait(sem);
   }

   void V(sem_t *sem)
   {
       sem_post(sem);
   }

public:
   Ring():p_step(0),c_step(0)
    {
        sem_init(&blank_sem, 0, NUM);
        sem_init(&data_sem, 0, 0);
    }

   void PushData(const int &data)
   {
       P(&blank_sem);
       ring[p_step] = data;
       p_step++;
       p_step %= NUM;
       V(&data_sem);
   }

   void PopData(int &data)
   {
       P(&data_sem);
       data = ring[c_step];
       c_step++;
       c_step %= NUM;
       V(&blank_sem);
   }

   ~Ring()
   {
       sem_destroy(&blank_sem);
       sem_destroy(&data_sem);
   }
};

void *producter(void *arg)
{
    Ring *r = (Ring*)arg;
    srand((unsigned long)time(NULL));
    while(1)
    {
        int data = rand() % 100 + 1;
        r->PushData(data);
        cout << "Product done : " << data << endl;
        sleep(1);
    }
}

void *consumer(void  *arg)
{
    Ring *r = (Ring*)arg;
    while(1)
    {
        int data;
        r->PopData(data);
        cout << "Consume done : " << data << endl;
        //sleep(1);
    }
}

int main()
{
    Ring r;
    pthread_t c, p;
    pthread_create(&c, NULL, consumer, (void *)&r);
    pthread_create(&p, NULL, producter, (void *)&r);




    pthread_join(c,NULL);
    pthread_join(p,NULL);

    return 0;
}
