#include "lib.h"
#include <vector>
#include <algorithm>
#include <functional>
using namespace std;
int main3()
{
    vector<thread> threads;
    for(int i=0;i<20;i++)
    {
        //临时对象如果不存下来，一条语句之后就消亡了
        //存下来就不会了，赋值给变量或者其他
        threads.push_back(thread(do_work,i));
    }
    //C++函数式编程
    for_each(threads.begin(),threads.end(),mem_fn(&thread::join));
    return 0;
}
