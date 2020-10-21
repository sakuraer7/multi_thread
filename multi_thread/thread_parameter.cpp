#include <iostream>
#include <thread>
#include <string>
#include "lib.h"
using namespace std;

void test(string const &s);
void change_string(string &s);
class X
{
public:
    void do_some_thing()
    {
        cout<<"Call this method.\n";
    }

};

int main()
{
    //第一种直接传值
    //此时，thread会把"hello"字面值自动转为string对象
    thread t1(test,"hello");
    //第二种传隐形转换
    char buffer[1024]={'b','u','f','f','e','r'};
    //此时thread无脑复制参数，导致复制过来的是char*类型，而不是我们想要的string
    //thread t2(test,buffer);
    //针对上面的问题，我们需要将其转换然后传参数
    thread t3(test,string(buffer));//string()这种通过类名产生的是临时对象，生命周期只有这一条语句
    //传引用
    string s="songhong";
    //此时，thread会复制这个数据然后使用复制数据的引用
    thread t4(change_string,ref(s));//这里应该是引用但是&引用报错，线程传引用只能用ref
    cout<<"main_s:"<<s<<endl;
    //使用类成员函数，线程使用成员函数，只能使用具体实例的成员函数
    X my_x;
    thread t5(&X::do_some_thing,&my_x);
    //thread对象也可以当参数进行传递，但是thread是可move不能copy的
    int a=10;
    thread_guard_1 tg((thread(func(a))));//将一个临时对象传过去

    t1.join();
    //t2.join();
    t3.join();
    t4.join();
    t5.join();

    return 0;
}
void test(string const &s)
{
    cout<<s<<endl;
}
void change_string(string &s)
{
    s="change";
    cout<<"t4_s:"<<s<<endl;
}
