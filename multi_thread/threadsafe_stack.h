#ifndef THREADSAFE_STACK_H
#define THREADSAFE_STACK_H

#include <exception>
#include <memory>
#include <stack>
#include <mutex>

struct empty_stack: std::exception
{
    const char* what() const throw()
    {
        return "empty stack.\n";
    }
};

template<typename T>
class threadsafe_stack
{
private:
    std::stack<T> data;
    mutable std::mutex m;
public:
    threadsafe_stack();
    threadsafe_stack(const threadsafe_stack& others);
    threadsafe_stack& operator=(const threadsafe_stack&) = delete;
    void push(T new_value);
    std::shared_ptr<T> pop();
    void pop(T& value);
    bool empty() const;
};

#endif // THREADSAFE_STACK_H
