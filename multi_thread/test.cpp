#include <iostream>
#include <thread>

void hello()
{
    std::cout<<"Hello world.\n"<<std::endl;
}
int main()
{
    std::thread t1(hello);
    std::cout<<std::endl;
    t1.join();
    return 0;
}