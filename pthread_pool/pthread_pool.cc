#include <iostream>
#include <queue>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

using namespace std;

int cal(int x, int y , int op);//函数太靠后，提前声明
typedef int (*HandlerTask_t)(int x, int y, int op);//定义一个函数指针，也是任务处理的方法

class Task//任务
{
private:
    int x;
    int y;
    int op;//0:+,1:-,2:*,3:/
    HandlerTask_t handler;
public:
    Task(int x_ = -1, int y_ = -1, int op_ = -1)
        :x(x_),y(y_),op(op_)
    {
    }

    void Register(HandlerTask_t handler_)//注册的方法传进来
    {
        handler = handler_;
    }

    void Run()//处理任务
    {
        int ret = handler(x, y, op);
        const char *str = "+-*/";
        cout << "thread is [ " << pthread_self() << " ] handler result : "<<  x <<  str[op] << y << " = " << ret << endl;
    }

    ~Task()
    {
    }
};


class ThreadPool//封装一个线程池
{
private:
    int thread_nums;//线程的总数
    int idle_nums;//当前有多少个任务空闲
    queue<Task> task_queue;//得有一个任务队列，临界资源，互斥,条件变量（本身有锁）
    pthread_mutex_t lock;//互斥
    pthread_cond_t cond;//条件变量，单方面通信，一个就行

public:
    void LockQueue()//上锁
    {
        pthread_mutex_lock(&lock);
    }

    void UnLockQueue()//解锁
    {
        pthread_mutex_unlock(&lock);
    }

    bool QueueIsEmpty()
    {
        return task_queue.size() == 0 ? true : false;
    }

    void ThreadIdle()
    {
        idle_nums++;//一个线程尝试去等的时候，意味着线程池中，等待的线程数目增多
        pthread_cond_wait(&cond, &lock);
        idle_nums--;//线程醒来的时候，说明线程池中的数目就减少了
    }

    void WakeupThread()
    {
        pthread_cond_signal(&cond);
    }


    void PopTask(Task &t)
    {
        t = task_queue.front();
        task_queue.pop();
    }

public:
    ThreadPool(int num_ =5):thread_nums(num_), idle_nums(0)
    {
        pthread_mutex_init(&lock, NULL);
        pthread_cond_init(&cond, NULL);
    }

    static void *ThreadRotinue(void *arg)//线程日常事务，被疯狂的重入   成员函数默认有this指针，改成static,this就只能自己传进去
    {
        pthread_detach(pthread_self());//线程分离
        ThreadPool *tp = (ThreadPool*)arg;

        while(1)
        {
            tp->LockQueue();
            while(tp->QueueIsEmpty())//先检测有没有任务
            {
                tp->ThreadIdle();
            }
            Task t;
            tp->PopTask(t);
            //Task t = task_queue.front();
            //task_queue.pop();
            tp->UnLockQueue();//把任务拿出来，事情就完成了，怎了处理是自己的事，先把锁释放，在处理事务的时候，别人也能进来
            t.Run();
        }
    }

    void InitThreadPool()//初始化线程池
    {
        pthread_t t;
        for(int i = 0; i < thread_nums; i++)
        {
            pthread_create(&t, NULL, ThreadRotinue, this);//创建线程    上面的static让我们自己传入进去
        }
    }

    void PushTask(const Task &t)//对外暴露一个接口任务接收任务
    {
        LockQueue();
        task_queue.push(t);
        WakeupThread();
        UnLockQueue();
    }

    ~ThreadPool()
    {
        pthread_mutex_destroy(&lock);
        pthread_cond_destroy(&cond);
    }
};

int cal(int x, int y , int op)//模拟一个计算器
{
    int ret = -1;
    switch(op)
    {
        case 0:
            ret = x + y;
            break;
        case 1:
            ret = x - y;
            break;
        case 2:
            ret = x * y;
            break;
        case 3:
            ret = x / y;
            break;
        default:
            cout << "cal error!" << endl;
            break; 
    }
}

int main()
{
    ThreadPool tp;
    tp.InitThreadPool();
    srand((unsigned long)time(NULL));//种下一个随机数种子
    for(;;)
    {
        sleep(1);
        int x = rand() % 100 + 1;
        int y = rand() % 100 + 1;
        int op = rand() % 4;
        Task t(x, y, op);
        t.Register(cal);
        tp.PushTask(t);
    }
    return 0;
}
