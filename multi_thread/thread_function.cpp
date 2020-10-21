#include <iostream>
#include <thread>
#include "lib.h"
std::thread f();
std::thread f(std::thread t);
int main4()
{
    //将f返回的临时变量赋予t1
    std::thread t1 = f();
    //函数返回的是临时变量t1时编译器会自动优化，不必手动调用move
    std::thread t2 = f(f());
    if(t1.joinable())
        t1.join();
    if(t2.joinable())
        t2.join();
    int some_local_state = 20;
    thread_guard_1 tg((std::thread(func(some_local_state))));
    cout<<std::thread::hardware_concurrency()<<endl;
    return 0;
}
std::thread f()
{
    return std::thread(do_some_function);
}
std::thread f(std::thread t)
{
    std::thread t1 = std::move(t);
    return t1;
}
