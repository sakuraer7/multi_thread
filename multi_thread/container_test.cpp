#include <iostream>
//class Y = T, 起个别名
template<typename T, class Y = T>
class X
{
public:
    X()
    {
        std::cout<<sizeof(Y)<<std::endl;
    }
};
int main10()
{
    X<int>();
    X<char>();
    X<long>();
    X<long long>();
    return 0;
}

