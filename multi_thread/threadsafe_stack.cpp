#include "threadsafe_stack.h"

template <typename T>
threadsafe_stack<T>::threadsafe_stack()
{
    data = std::stack<T> ();
}
template <typename T>
void threadsafe_stack<T>::push(T new_value)
{
    std::lock_guard<std::mutex> guard(m);
    data.push(new_value);
}
template <typename T>
std::shared_ptr<T> threadsafe_stack<T>::pop()
{
    std::lock_guard<std::mutex> guard(m);
    if(data.empty())
        throw empty_stack();
    std::shared_ptr<T> const res(std::make_shared<T>(data.top()));
    data.pop();
    return res;
}
template <typename T>
void threadsafe_stack<T>::pop(T& value)
{
    std::lock_guard<std::mutex> guard(m);
    if(data.empty())
        throw empty_stack();
    value = data.top();
    data.pop();
}
template <typename T>
bool threadsafe_stack<T>::empty() const
{
    std::lock_guard<std::mutex> guard(m);
    return data.empty();
}
template <typename T>
threadsafe_stack<T>::threadsafe_stack(const threadsafe_stack& others)
{
    std::lock_guard<std::mutex> guard(m);
    data = others.data;
}

