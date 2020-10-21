#include <iostream>
#include <thread>
#include <future>
#include <vector>

int calSum(int& n)
{
    int sum = 0;
    while(n)
    {
        sum += n;
        n--;
    }
    return sum;
}
void test(int& n)
{
    n--;
}
int main()
{
    int n = 100;
    int m = 100;
    int& k = m;
    //异步传引用
    std::future<int> answer = std::async(calSum, std::ref(n));
    std::cout<<"The answer is "<<answer.get()<<".\n";
    std::cout<<"n = "<<n<<".\n";
    //线程传引用
    std::thread t(test, std::ref(k));
    t.join();
    std::cout<<"m = "<<m<<".\n";
    return 0;
}