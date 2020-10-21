#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <functional>
#include <queue>

class join_threads
{
private:
    std::vector<std::thread> &threads;
public:
    join_threads(std::vector<std::thread> &threads);
    ~join_threads();
};
class ThreadPool
{
private:
    std::atomic<bool> done;
    //此处是线程安全队列，这里为了方便没有使用
    std::queue<std::function<void()>> work_queue;
    std::vector<std::thread> threads;
    join_threads joiner;
public:
    ThreadPool(/* args */);
    ~ThreadPool();
    void work_thread();
    //往工作队列添加任务
    template<typename Functype>
    void submit(Functype f)
    {
        work_queue.emplace(std::function<void(void)>(f));
    }
};
int main()
{
    using std::cout;
    cout<<"Hello world!\n";
    return 0;
}
join_threads::join_threads(std::vector<std::thread> &other_threads): 
    threads(other_threads)
{}

join_threads::~join_threads()
{
    for(int i=0; i<threads.size(); i++)
    {
        if(threads[i].joinable())
            threads[i].join();
    }
}
ThreadPool::ThreadPool(/* args */): done(false), joiner(threads)
{
    unsigned const long thread_count = std::thread::hardware_concurrency();
    try
    {
        for(int i=0; i<thread_count; i++)
        {
            threads.emplace_back(std::thread(&ThreadPool::work_thread, this));
        }
    }
    catch(const std::exception& e)
    {
        //构造函数抛出异常，析构函数不会执行，
        //所以这里要处理done，否则已经添加的线程会永远执行下去
        done = true;
        throw;
    }
    
}

ThreadPool::~ThreadPool()
{
    done = true;
}
void ThreadPool::work_thread()
{
    //一个任务完成，就从工作队列取新任务，直到done置为true
    while(!done)
    {
        std::function<void(void)> task;
        if(!work_queue.empty())
        {
            task = work_queue.front();
            work_queue.pop();
            task();
        }
        else
        {
            std::this_thread::yield();
        }
    }
}
