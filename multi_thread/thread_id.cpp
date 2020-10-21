#include <thread>
#include <iostream>

void some_core_part_of_algorithm();
std::thread::id master_thread;
int main()
{
    //this_thread::get_id()是获得当前线程的id
    master_thread = std::this_thread::get_id();
    std::thread t1(some_core_part_of_algorithm);
    //thread.get_id()是获取此thread对象的id
    std::cout<<t1.get_id()<<std::endl;
    t1.join();
    some_core_part_of_algorithm();
    std::this_thread::get_id();
    return 0;
}
void some_core_part_of_algorithm()
{
    if(std::this_thread::get_id() == master_thread)
        std::cout<<"This is master thread. id = "<<master_thread<<std::endl;
    else
        std::cout<<"This is not master thread. id = "<<std::this_thread::get_id()<<std::endl;
}
