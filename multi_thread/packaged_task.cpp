#include <iostream>
#include <future>
#include <thread>
#include <deque>
#include <utility>
#include <mutex>

std::mutex m;
std::deque<std::packaged_task<void()> > tasks;
bool gui_shutdown = false;
void get_and_process_gui_message(){}
void gui_thread();
template<typename Func>
std::future<void> post_task_for_gui_thread(Func f);
int main()
{

    return 0;
}
void gui_thread()
{
    while(!gui_shutdown)
    {
        get_and_process_gui_message();
        std::packaged_task<void()> task;
        {
            std::lock_guard<std::mutex> lk(m);
            if(tasks.empty())
                continue;
            task = std::move(tasks.front());
            tasks.pop_front();
        }
        //do_task();
    }
}
template<typename Func>
std::future<void> post_task_for_gui_thread(Func f)
{
    std::packaged_task<void()> task(f);
    std::future<void> res = task.get_future();
    std::lock_guard<std::mutex> lk(m);
    tasks.push_back(std::move(task));
    return res;
}