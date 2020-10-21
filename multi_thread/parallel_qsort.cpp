#include <iostream>
#include <thread>
#include <mutex>
#include <memory>
#include <future>
#include <atomic>
#include <stack>
#include <vector>
#include <list>
#include <exception>
#include <chrono>
#include <utility>
#include <algorithm>

template<typename T>
class threadsafe_stack
{
private:
    mutable std::mutex m;
    std::stack<std::shared_ptr<T> > data;
    struct empty_stack: public std::exception
    {
        const char* what() const throw()
        {
            return "empty stack.\n";
        }
    };
    
public:
    threadsafe_stack(const threadsafe_stack& other);
    threadsafe_stack& operator=(const threadsafe_stack&) = delete;
    void pop(T& value);
    std::shared_ptr<T> pop();
    void push(T& new_value);
    bool empty();
    ~threadsafe_stack();
};
template<typename T>
class sorter
{
private:
    struct chunk_to_sort
    {
        std::list<T> data;
        std::promise<std::list<T> > promise;
    };
    threadsafe_stack<chunk_to_sort> chunks;
    std::vector<std::thread> threads;
    unsigned const max_thread_count;
    std::atomic<bool> end_of_data;
public:
    sorter(): max_thread_count(std::thread::hardware_concurrency()-1),
    end_of_data(false)
    {}
    ~sorter()
    {
        end_of_data = true;
        for(int i=0; i<threads.size(); i++)
        {
            if(threads[i].joinable())
                threads[i].join();
        }
    }
    void try_sort_chunk();
    std::list<T> do_sort(std::list<T>& chunk_data);
    void sort_chunk(std::shared_ptr<chunk_to_sort> const &chunk);
    void sort_thread();
};
int main()
{
    std::cout<<"parallel quick sort.\n";
    return 0;
}
template<typename T>
threadsafe_stack<T>::threadsafe_stack(const threadsafe_stack& other)
{
    std::lock_guard<std::mutex> lk(m);
    data = other.data;
}
template<typename T>
threadsafe_stack<T>::~threadsafe_stack(){}
template<typename T>
bool threadsafe_stack<T>::empty()
{
    std::lock_guard<std::mutex> lk(m);
    return data.empty();
}
template<typename T>
void threadsafe_stack<T>::push(T& new_value)
{
    std::shared_ptr<T> new_ptr = std::make_shared<T>(new_value);
    std::lock_guard<std::mutex> lk(m);
    data.emplace(std::move(new_ptr));
}
template<typename T>
void threadsafe_stack<T>::pop(T& value)
{
    if(this->empty())
        throw empty_stack();
    std::lock_guard<std::mutex> lk(m);
    value = *data.top();
    data.pop();
}
template<typename T>
std::shared_ptr<T> threadsafe_stack<T>::pop()
{
    if(this->empty())
        throw empty_stack();
    std::lock_guard<std::mutex> lk(m);
    std::shared_ptr<T> res = data.top();
    data.pop();
    return res;
}
template<typename T>
void sorter<T>::try_sort_chunk()
{
    std::shared_ptr<chunk_to_sort> chunk = chunks.pop();
    if(chunk)
        sort_chunk(chunk);
}
template<typename T>
//算法核心，分割数据，交给不同的线程处理
std::list<T> sorter<T>::do_sort(std::list<T>& chunk_data)
{
    if(chunk_data.empty())
        return chunk_data;
    std::list<T> result;
    result.splice(result.begin(), chunk_data, chunk_data.begin());
    T const& partition_val = *result.begin();

    typename std::list<T>::iterator divide_point = 
    std::partition(chunk_data.begin(), chunk_data.end(), 
    [&](T const& val)->bool{return val < partition_val;});

    chunk_to_sort new_lower_chunk;
    new_lower_chunk.data.splice(new_lower_chunk.data.end(),
    chunk_data, chunk_data.begin(), divide_point);

    std::future<std::list<T> > new_lower = 
    new_lower_chunk.promise.get_future();
    
    chunks.push(new_lower_chunk);
    if(threads.size() < max_thread_count)
    { 
        threads.emplace_back(std::thread(&sorter<T>::sort_thread, this));
    }
    std::list<T> new_higher_chunk(do_sort(chunk_data));
    result.splice(result.end(), new_higher_chunk);

    while(new_lower.wait_for(std::chrono::seconds(0)) != 
    std::future_status::ready)
    {
        try_sort_chunk();
    }
    result.splice(result.begin(), new_lower.get());
    return result;
}
template<typename T>
void sorter<T>::sort_chunk(std::shared_ptr<chunk_to_sort> const &chunk)
{
    chunk->promise.set_value(do_sort(chunk->data));
}
template<typename T>
void sorter<T>::sort_thread()
{
    while(!end_of_data)
    {
        try_sort_chunk();
        std::this_thread::yield();
    }
}
