#include <iostream>
#include <list>
#include <vector>
#include <algorithm>

int quick_sort(std::vector<int>& input)
{
    std::sort(input.begin(), input.end(), [&](const int& a, const int& b){return a > b;});
    return *(input.begin());
}
int main()
{
    std::vector<int> vector;
    for(int i=0; i<20; i++)
    {
        vector.emplace_back((i*13+7)%5);
    }
    //auto可以自动推断类型创建变量
    auto max(quick_sort(vector));
    for(auto& num : vector)
    {
        std::cout<<num<<" ";
    }
    std::cout<<"\n";
    std::cout<<max<<"\n";
    for(auto& num : vector)
    {
        std::cout<<num<<" ";
    }
    std::cout<<"\n";
    return 0;
}