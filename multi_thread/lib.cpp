#include "lib.h"
#include <iostream>
using namespace std;

thread_guard::thread_guard(thread &t_):t(t_)
{}
thread_guard::~thread_guard()
{
    if(t.joinable())
        t.join();
}
thread_guard_1::thread_guard_1(thread t_):t(move(t_))
{
    if(!t.joinable())
        throw logic_error("No thread.");
}
thread_guard_1::~thread_guard_1()
{
    t.join();
    cout<<"Call the ~thread_guard_1().\n";
}
void func::operator()()
{
    for(unsigned j=0;j<1000000;++j)
    {
        //ToDo
    }
}
func::func(int &i_):i(i_)
{}
void do_work(unsigned id)
{
    cout<<id<<endl;
}
void do_some_function()
{
    cout<<"Call do_some_function\n";
}
void do_some_other_function(int n)
{
    cout<<"Call do_some_other_function. n="<<n<<endl;
}
