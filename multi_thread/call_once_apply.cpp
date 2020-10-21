#include <iostream>
#include <thread>
#include <mutex>
#include <memory>

std::once_flag flag;
void do_init()
{
    std::cout<<"Do init.\n";
}
void do_once()
{
    std::call_once(flag, do_init);
}
int main8()
{
    std::thread t1(do_once);
    std::thread t2(do_once);
    std::thread t3(do_once);
    std::thread t4(do_once);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    std::thread t5(do_once);
    std::thread t6(do_once);
    t5.join();
    t6.join();
    //shared_ptr用法
    std::shared_ptr<int> ptr = std::make_shared<int>(10);
    auto q(ptr);
    std::cout<<"*ptr = "<<*ptr<<"*q = "<<*q<<"q.count = "<<q.use_count()<<ptr.use_count()<<std::endl;
    q = std::make_shared<int>(40);
    std::cout<<ptr.use_count()<<std::endl;
    return 0;
}
