#include <iostream>
#include <thread>
using namespace std;
void hello()
{
    cout<<"Hello Concurrent world!\n";
}
void do_some_work(int i);
class background_task
{
public:
//重载()表明类的callable
    void operator() () const
    {
        cout<<"Do something.\n";
    }
};
class thread_guard
{
    thread &t;
public:
    explicit thread_guard(thread &t_):t(t_)
    {}
    //使用析构函数来实现线程的等待，因为主线程结束销毁对象时会调用析构函数
    ~thread_guard()
    {
        if(t.joinable())
            t.join();
        cout<<"Call this method.\n";
    }
    thread_guard(thread_guard const&)=delete;
    thread_guard& operator=(thread_guard const&)=delete;
};
struct func
{
    int &i;
    func(int &i_):i(i_)
    {}
    void operator() ()
    {
        for(unsigned j=0;j<1000000;++j)
        {
            do_some_work(i);
        }
    }
};

int main1()
{
    thread t(hello);
    background_task f;
    thread t1(f);
    //共享变量
    int some_local_state=0;
    //在多线程编程时尽量不要使用共享变量，这样会导致主进程结束，此变量结束了生命周期
    //子进程还在使用它
    func my_func(some_local_state);
    thread t2(my_func);
    thread_guard g(t2);
    //将thread与一个对象绑定，这样本线程（主线程）结束时必定会调用join方法。
    //因为我们在析构函数实现了它
    t.join();
    t1.join();
    return 0;
}
void do_some_work(int i)
{
    //cout<<"i="<<i<<endl;
}
