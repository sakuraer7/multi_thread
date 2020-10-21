#include <iostream>
#include <algorithm>

template<typename T>
T myMax(T a)
{
    return a;
}
template<typename T, typename... Args>
T myMax(T a, Args... args)
{
    T max_num = myMax(args...);
    return a > max_num ? a : max_num;
}
int main()
{

    std::cout<<myMax<int>(1, 2, 3, 4, 5, 6)<<"\n";
    return 0;
}