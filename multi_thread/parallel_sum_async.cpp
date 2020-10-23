#include <iostream>
#include <algorithm>
#include <thread>
#include <future>
#include <vector>
#include <numeric>
#include <iterator>

//使用async实现一个并行求和算法
template<typename Iterator, typename T>
T parallel_sum(Iterator begin, Iterator end, T init)
{
    const int length = std::distance(begin, end);
    if(!length)
        return init;
    const int max_per_thread = 25;
    if(length <= max_per_thread)
    {
        return std::accumulate(begin, end, init);
    }
    Iterator mid_point = begin;
    std::advance(mid_point, length/2);
    std::future<T> first_half_res = std::async(parallel_sum<Iterator, T>, begin, mid_point, T());
    T second_half_res = parallel_sum(mid_point, end, init);
    return first_half_res.get() + second_half_res;
}
int main()
{
    using namespace std;
    std::vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
    cout<<"Parallel sum async.\n";
    cout<<"sum = "<<parallel_sum<vector<int>::iterator, int>(nums.begin(), nums.end(), 0)<<"\n";
}