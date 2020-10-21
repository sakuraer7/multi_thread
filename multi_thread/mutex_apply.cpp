#include <iostream>
#include <thread>
#include <mutex>
#include <algorithm>
#include <list>

std::list<int> some_list;
std::mutex some_mutex;
void add_list(int value_to_add);
bool list_contain(int value_to_find);
template<typename T>
//使用右值引用必须声明为模板，T&&表示类型自动推断，而不是单纯的右引用，若是具体类型则为右值引用
void print_num(T&&);
void add(int&);
void sub(int&);
void add_safe(int&);
void sub_safe(int&);
int main()
{
    int a = 0;
    const int& b = 1;
    int& c = a;
    //move可以将左引用变为右引用
    print_num(a);
    print_num(c);
    print_num(b);
    print_num(2);
    //互斥应用
    int m = 0, n = 0;
    std::thread t1(add, std::ref(m));
    std::thread t2(sub, std::ref(m));
    
    std::thread t3(add_safe, std::ref(n));
    std::thread t4(sub_safe, std::ref(n));
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    //等待线程执行完毕，输出结果
    std::cout<<"no mutex: m = "<<m<<std::endl;
    std::cout<<"mutex: n = "<<n<<std::endl;
    return 0;
}
//lock_grad类似于thread_guard将lock与类绑定，保证unlock的调用
//将add和contain函数隔离开，保证互相不受干扰
void add_list(int value_to_add)
{
    std::lock_guard<std::mutex> guard(some_mutex);
    some_list.push_back(value_to_add);
}
bool list_contain(int value_to_find)
{
    std::lock_guard<std::mutex> guard(some_mutex);
    return std::find(some_list.begin(), some_list.end(), value_to_find) 
    != some_list.end();
}
template<typename T>
void print_num(T&& a)
{
    std::cout<<"call this "<<a<<std::endl;
}
void add(int& m)
{
    for(int i=0; i<1000000; i++)
        m = m + 1;
}
void sub(int& m)
{
    for(int i=0; i<1000000; i++)
        m = m - 1;
}
void add_safe(int& m)
{
    for(int i=0; i<1000000; i++)
    {
        std::lock_guard<std::mutex> guard(some_mutex);
        m = m + 1;
    }
}
void sub_safe(int& m)
{
    for(int i=0; i<1000000; i++)
    {
        std::lock_guard<std::mutex> guard(some_mutex);
        m = m - 1;
    }
}
