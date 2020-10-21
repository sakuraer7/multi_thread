#include <iostream>

struct Adder
{
    int count = 0;
    Adder& operator << (const int& value)
    {
        count += value;
        return *this;
    }
    friend std::ostream& operator<< (std::ostream& os, const Adder& addder)
    {
        os<<addder.count;
        return os;
    }
};
int main()
{
    Adder a;
    a<<1;
    std::cout<<a<<"\n";
    a<<1<<2<<3;
    std::cout<<a<<"\n";
    return 0;
}