#include <iostream>
#include <thread>
#include <queue>
#include <future>
#include <vector>
#include <memory>

class FunctionWrapper
{
private:
    struct impl_base
    {
        virtual void call() = 0;
        virtual ~impl_base(){}
    };
    std::unique_ptr<impl_base> impl;
    template<typename F>
    struct impl_type: public impl_base
    {
        F f;
        impl_type(F&& _f): f(std::move(_f))
        {}
        void call()
        {
            f();
        }
    };
public:
    template<typename F>
    FunctionWrapper(F&& f): impl(new impl_type<F>(std::move(f)))
    {}
    void operator()()
    {
        impl->call();
    }
    FunctionWrapper() = default;
    
    FunctionWrapper(FunctionWrapper&& function): impl(std::move(function.impl))
    {}
    FunctionWrapper& operator=(FunctionWrapper&& others)
    {
        this->impl = std::move(others.impl);
        return *this;
    }
    FunctionWrapper(FunctionWrapper&) = delete;
    FunctionWrapper(const FunctionWrapper&) = delete;
    FunctionWrapper& operator=(FunctionWrapper&) = delete;
    ~FunctionWrapper(){}
};
class ThreadPool
{
private:
    std::atomic<bool> done;
    std::queue<FunctionWrapper> work_queue;
    void work_thread()
    {
        while(!done)
        {
            FunctionWrapper function;
            if(!work_queue.empty())
            {
                function = std::move(work_queue.front());
                work_queue.pop();
            }
            else
            {
                std::this_thread::yield();
            }
        }
    }
public:
    ThreadPool(/* args */);
    ~ThreadPool();
    template<typename Functype>
    //返回一个future允许等待线程池中的任务完成(调用get方法)
    //不返回future的线程池则是不知道什么时候任务结束，只能等到join之后才知道
    std::future<typename std::result_of<Functype()>::type> submit(Functype f)
    {
        typedef typename std::result_of<Functype()>::type result_type;
        std::packaged_task<result_type()> task(std::move(f));
        std::future<result_type> res(task.get_future());
        work_queue.emplace(std::move(task));
        return res;
    }
};

ThreadPool::ThreadPool(/* args */)
{
}

ThreadPool::~ThreadPool()
{
}

int main()
{
    using std::cout;
    cout<<"Wait submit thread pool.\n";
    return 0;
}
