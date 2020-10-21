#include <iostream>

struct test
{
    //按类型分配，只不过会截断
    int a : 10;
    int b : 25;
    unsigned c;
};
int compare(void* a, void* b)
{
    return *((int*)a) - *((int*)b);
}

using t = test;
int main()
{
    t a{1024, 1024, 1024};
    std::cout<<a.a<<" "<<a.b<<" "<<a.c<<"\n";//0 1024 1024
    std::cout<<sizeof(a)<<"\n";//12
    std::cout<<sizeof(a.c)<<"\n";//4
    int x = 10, y = 20;
    std::cout<<compare(&x, &y)<<"\n";
}