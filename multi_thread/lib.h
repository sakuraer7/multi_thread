#ifndef LIB_H
#define LIB_H
#include <thread>
using namespace std;
struct func
{
    int &i;
    func(int &i_);
    void operator() ();
};
class thread_guard
{
    thread &t;
public:
    explicit thread_guard(thread &t_);
    //使用析构函数来实现线程的等待，因为主线程结束销毁对象时会调用析构函数
    ~thread_guard();
    thread_guard(thread_guard const&)=delete;
    thread_guard& operator=(thread_guard const&)=delete;
};
class thread_guard_1
{
    thread t;
public:
    explicit thread_guard_1(thread t_);
    ~thread_guard_1();
    thread_guard_1(thread_guard_1 const&)=delete;
    thread_guard_1& operator=(thread_guard_1 const&)=delete;
};
void do_work(unsigned id);
void do_some_function();
void do_some_other_function(int n);
#endif // LIB_H
