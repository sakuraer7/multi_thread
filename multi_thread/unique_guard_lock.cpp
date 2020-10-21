#include <iostream>
#include <mutex>

int main9()
{
    std::mutex guard_mutex;
    std::mutex unqiue_mutex;
    //guard_lock构造函数加锁，析构函数解锁，不能转移所有权，比较“笨重”
    std::lock_guard<std::mutex> g_lock(guard_mutex);
    //unique_lock可以不在构造函数中加锁，延迟加锁，能转移所有权，比较灵活，代价就是更多资源的占用
    std::unique_lock<std::mutex> u_lock(unqiue_mutex, std::defer_lock);
    u_lock.try_lock();
    return 0;
}
