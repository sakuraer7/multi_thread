#include <iostream>
#include <chrono>
#include <future>

int some_task()
{
    //当前线程休眠40ms
    std::this_thread::sleep_for(std::chrono::milliseconds(40));
    return 100;
}
int main()
{
    //毫秒
    std::chrono::milliseconds ms(1111);
    std::cout<<ms.count()<<"\n";
    //小转大会截断小数点后的部分
    std::chrono::seconds s = std::chrono::duration_cast<std::chrono::seconds>(ms);
    s = s * 5;
    std::cout<<s.count()<<"\n";
    auto start = std::chrono::steady_clock::now();
    std::future<int> future = std::async(some_task);
    //等待39ms之后future是否就绪
    if(future.wait_for(std::chrono::milliseconds(39)) == std::future_status::ready)
    {
        std::cout<<"future.get() is reday, value is "<<future.get()<<"\n";
    }
    else
    {
        std::cout<<"future.get() is timeout.\n";
    }
    auto end = std::chrono::steady_clock::now();
    double time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout<<"wait "<<time<<" microseconds.\n";
    return 0;
}