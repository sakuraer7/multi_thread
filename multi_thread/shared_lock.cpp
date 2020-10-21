#include <map>
#include <mutex>
#include <string>
#include <boost/thread/shared_mutex.hpp>

class dns_entry
{

};

class dns_cache
{
    //使用shared_lock和shared_mutex实现一个读者-写者锁
    std::map<std::string, dns_entry> entries;
    mutable boost::shared_mutex entry_mutex;
public:
    dns_entry find_entry(std::string const &domin)
    {
        //获取shared_lock，允许多个读
        boost::shared_lock<boost::shared_mutex> lk(entry_mutex);
        std::map<std::string, dns_entry>::const_iterator iter =
                entries.find(domin);
        return iter == entries.end() ? dns_entry() : iter->second;
    }
    void update_or_add_entry(std::string const &domin,
                                  dns_entry const &entry_details)
    {
        //写时，获取lock唯一锁
        std::lock_guard<boost::shared_mutex> lk(entry_mutex);
        entries[domin] = entry_details;
    }
};
