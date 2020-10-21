#include <thread>
#include <vector>
#include <numeric>
#include <functional>
#include <algorithm>
#include <limits>
#include <iostream>
#include <ctime>

template<typename Iterator,typename T>
struct accumulate_block
{
    void operator()(Iterator first,Iterator last,T& result)
    {
        result=std::accumulate(first,last,result);
    }
};
template<typename Iterator,typename T>
T parallel_accumulate(Iterator first,Iterator last,T init)
{
    unsigned long const length=std::distance(first,last);
    if(!length) // 1
    return init;
    unsigned long const min_per_thread=25;//每个线程处理的数据个数
    unsigned long const max_threads=
    (length+min_per_thread-1)/min_per_thread; //计算所有数据需要的最大线程数，这个计算方式不是很懂
    unsigned long const hardware_threads=
    std::thread::hardware_concurrency();
    unsigned long const num_threads=        //结合硬件得到线程数
    std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
    unsigned long const block_size=length/num_threads;  //根据线程数得到每一个块大小
    std::vector<T> results(num_threads);//存储每个线程运算完的临时结果
    std::vector<std::thread> threads(num_threads-1);    //创建线程容器，减1是因为有个主线程
    Iterator block_start=first;
    for(unsigned long i=0; i < (num_threads-1); ++i)
    {
        //创建多线程
        Iterator block_end=block_start;
        std::advance(block_end,block_size);// 6
        threads[i]=std::thread(     // 7
            accumulate_block<Iterator,T>(),
            block_start,block_end,std::ref(results[i]));//进程传引用需用ref
        block_start=block_end;      // 8每个线程将自己负责的数据累加存储到对应的results中
    }
    accumulate_block<Iterator,T>()(
    block_start,last,results[num_threads-1]); //9数据不可能总是线程数整数倍，处理余下数据存到results中
    std::for_each(threads.begin(),threads.end(),
    std::mem_fn(&std::thread::join));   // 10等待所有线程结束
    return std::accumulate(results.begin(),results.end(),init); // 11再次累加得到最终结果
}
int main()
{
    std::vector<int> v;
    for(int i=0; i<219999999;i++)
        v.push_back(i%20);
    long long sum = 0;
    std::clock_t start, end;
    start = std::clock();
    //模板函数调用
    sum = parallel_accumulate<std::vector<int>::iterator, int>(v.begin(),v.end(),0);
    end = std::clock();
    std::cout<<"parallel thread: sum="<<sum<<" time="<<(double)(end-start)/CLOCKS_PER_SEC<<std::endl;
    sum = 0;
    start = std::clock();
    for(size_t i=0; i<v.size(); i++)
    {
        sum+=v[i];
    }
    end = std::clock();
    std::cout<<"direct sum: sum="<<sum<<" time="<<(double)(end-start)/CLOCKS_PER_SEC<<std::endl;
    sum = 0;
    start = std::clock();
    sum = std::accumulate(v.begin(), v.end(), 0);
    end = std::clock();
    std::cout<<"accumulate: sum="<<sum<<" time="<<(double)(end-start)/CLOCKS_PER_SEC<<std::endl;
    return 0;
}
