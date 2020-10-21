#include <iostream>
#include <thread>
#include <future>
#include <exception>

//packaged_task提供期望与函数的绑定，promise则提供数值与期望的绑定
void new_thread(std::future<int>& future)
{
    std::cout<<"value is "<<future.get()<<"\n";
}
void excep_thread(std::future<int>& future)
{
    try
    {
        std::cout<<"value is "<<future.get()<<"\n";
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }       
}
int main()
{
    std::promise<int> pro;
    std::future<int> future = pro.get_future();
    //将期望传给需要的线程
    std::thread t1(new_thread, std::ref(future));
    //promise设置future的值然后变为就绪
    pro.set_value(50);//此时new_thread收到50
    std::promise<int> promise;
    std::future<int> excep_future = promise.get_future();
    //设置异常，使用future的线程在自己线程中捕获异常
    std::thread t2(excep_thread, std::ref(excep_future));
    try
    {
        throw std::exception();
    }
    catch(const std::exception& e)
    {
        promise.set_exception(std::current_exception());
    }
    t1.join();
    t2.join();
    return 0;
}