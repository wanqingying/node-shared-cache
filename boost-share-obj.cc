// include
#include <iostream>
#include <variant>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/containers/pair.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/segment_manager.hpp>
#include <boost/interprocess/sync/named_sharable_mutex.hpp>
#include <boost/interprocess/sync/named_condition.hpp>
#include "help.cc"

namespace dtl = boost::container::dtl;
namespace bip = boost::interprocess;

typedef bip::allocator<char, bip::managed_shared_memory::segment_manager> CharAllocator;
typedef bip::basic_string<char, std::char_traits<char>, CharAllocator> ShmString;
typedef bip::allocator<ShmString, bip::managed_shared_memory::segment_manager> StringAllocator;
typedef ShmString PKeyType;
// std.pair
template <typename V>
using PairTemp = std::pair<const ShmString, V>;

typedef std::variant<bool, double, ShmString, std::string> TDataType;

// typedef <long,TDataType> bip pair
typedef bip::pair<long, TDataType> PValueType;

typedef PairTemp<PValueType> TPairType;
typedef bip::allocator<TPairType, bip::managed_shared_memory::segment_manager> ShmAllocator;
typedef bip::map<PKeyType, PValueType, std::less<ShmString>, ShmAllocator> MyMap;
// memory page size

int getDataSize(TDataType &v)
{
    if (std::holds_alternative<bool>(v))
    {
        return sizeof(bool);
    }
    else if (std::holds_alternative<double>(v))
    {
        return sizeof(double);
    }
    else if (std::holds_alternative<std::string>(v))
    {
        return std::get<std::string>(v).size();
    }
    else if (std::holds_alternative<ShmString>(v))
    {
        return std::get<ShmString>(v).size();
    }
    return 0;
}

TDataType buildType(bool v)
{
    return v;
}

TDataType buildType(double v)
{
    return v;
}

TDataType buildType(std::string v)
{
    return v;
}

TDataType buildType(ShmString v)
{
    return v;
}

class BoostNamedShareMutex
{
private:
    bip::named_sharable_mutex *mutex;
    std::string name;

public:
    bool enable;
    nnd::ShmLog *slog = new nnd::ShmLog();

public:
    BoostNamedShareMutex(std::string name, bool renew = true, bool enable = true)
    {
        this->name = name;
        this->enable = enable;
        if (renew)
        {
            bip::named_sharable_mutex::remove(name.c_str());
        }
        mutex = new bip::named_sharable_mutex(bip::open_or_create, name.c_str());
    }
    ~BoostNamedShareMutex()
    {
        try
        {
            bip::named_sharable_mutex::remove(name.c_str());
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
    }
    void lock()
    {
        if (enable)
        {
            mutex->lock();
        }
    }
    void unlock()
    {
        if (enable)
        {
            mutex->unlock();
        }
    }
    void lock_sharable()
    {
        if (enable)
        {
            mutex->lock_sharable();
        }
    }
    void unlock_sharable()
    {
        if (enable)
        {
            mutex->unlock_sharable();
        }
    }
    void setEnable(bool enable)
    {
        this->enable = enable;
    }
};

class BoostShareMap
{
    // linux memory page size
    static const long BP_SIZE = 1024 * 4;
    // max alloc memory size 4G
    // static const int FULL_MAX_SIZE = 1024 * 4 * 1024 * 1024;

private:
    /* data */
    bip::managed_shared_memory *managed_shm;
    // bip::segment_manager
    // bip::segment_manager *sm;
    MyMap *mymap;
    std::string shm_name;
    int shm_size;
    // 4GB
    long max_size = BP_SIZE * 1024 * 1024;
    BoostNamedShareMutex *share_mutex;
    bool isMaster = false;
    // milliseconds
    long max_age = 2000;
    long last_clean_time = nnd::get_current_time_millis();

public:
    nnd::ShmLog *slog = new nnd::ShmLog();

public:
    BoostShareMap(std::string name, int size, bool renew = false)
    {
        // print init
        std::cout << "BoostShareMap init" << std::endl;
        std::string mtx_name = shm_name + "_mtx";
        shm_name = name;
        shm_size = size;
        this->share_mutex = new BoostNamedShareMutex(mtx_name);
        if (renew)
        {
            bip::shared_memory_object::remove(shm_name.c_str());
        }
        try
        {
            managed_shm = new bip::managed_shared_memory(bip::open_or_create, name.c_str(), size);
            mymap = managed_shm->find_or_construct<MyMap>("MyMap")(managed_shm->get_segment_manager());
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
    }
    ~BoostShareMap()
    {
        if (isMaster)
        {
            // remove shared memory
            bip::shared_memory_object::remove(shm_name.c_str());
        }
    }

    void insert(std::string &key, TDataType &value, bool retry = true, long max_age = 2000)
    {
        // key.size +value.size
        int pairSize = key.size() + getDataSize(value);
        try
        {
            this->share_mutex->lock();
            auto *sm = managed_shm->get_segment_manager();
            PValueType pv;
            long expire_at = nnd::get_current_time_millis() + max_age;
            if (std::holds_alternative<bool>(value))
            {
                pv = std::make_pair(expire_at, std::get<bool>(value));
            }
            else if (std::holds_alternative<double>(value))
            {
                pv = std::make_pair(expire_at, std::get<double>(value));
            }
            else if (std::holds_alternative<std::string>(value))
            {
                pv = std::make_pair(expire_at, ShmString(std::get<std::string>(value).c_str(), sm));
            }
            else
            {
                this->slog->debug("insert error value type");
                return;
            }

            TPairType pair = std::make_pair(ShmString(key.c_str(), sm), pv);

            auto newPair = mymap->insert(pair);
            if (!newPair.second)
            {
                mymap->erase(pair.first);
                mymap->insert(pair);
            }
            this->share_mutex->unlock();
        }
        catch (const std::exception &e)
        {

            this->share_mutex->unlock();
            // debug + key
            this->slog->debug("handleInsertLost key=", key);

            this->handleInsertLost(pairSize);

            if (retry)
            {
                this->slog->debug("retry insert key=", key);
                this->insert(key, value, false);
            }
            else
            {
                this->slog->error("insert error key=", key);
            }
        }
    }
    void insert(const std::string &key, TDataType &value)
    {
        std::string k = key;
        this->insert(k, value);
    }
    void insert(std::string key, double value)
    {
        TDataType v = buildType(value);
        this->insert(key, v);
    }
    void insert(std::string key, bool value)
    {
        TDataType v = buildType(value);
        this->insert(key, v);
    }
    void insert(std::string key, std::string value)
    {
        TDataType v = buildType(value);
        this->insert(key, v);
    }
    void insert(std::string &key, std::string &value)
    {
        TDataType v = buildType(value);
        this->insert(key, v);
    }
    void print()
    {
        // print map
        for (auto it = mymap->begin(); it != mymap->end(); it++)
        {
            // std::cout << it->first << " " << it->second.second << std::endl;
        }
    }

    void cleanKeys()
    {
        long now_t = nnd::get_current_time_millis();
        if (now_t - last_clean_time < 1000)
        {
            return;
        }

        this->slog->debug("start cleanKeys free=", std::to_string(this->managed_shm->get_free_memory()));
        this->share_mutex->lock();

        for (auto it = mymap->begin(); it != mymap->end();)
        {

            PValueType v;
            //  v = it->second;
            try
            {
                v = it->second;
            }
            catch (const std::exception &e)
            {
                this->slog->error("cleanKeys error key=", it->first.c_str());
                it++;
                continue;
            }
            long expireAt = v.first;
            if (now_t > expireAt)
            {
                mymap->erase(it++);
            }
            else
            {
                it++;
            }
        }
        this->share_mutex->unlock();
        unsigned long freeSize = this->managed_shm->get_free_memory();
        this->last_clean_time = now_t;
        long total_size = this->managed_shm->get_size();
        long free_size = this->managed_shm->get_free_memory();
        this->slog->debug("start cleanKeys free=", std::to_string(free_size));
        if (free_size > total_size / 2 && total_size > BP_SIZE * 1024 * 10)
        {
            // if free size > 50% and total size > 40MB means there is more than 20MB to free
            this->autoShrink();
        }
    }
    void forceCleanKeys(long min_size)
    {
        this->slog->debug("forceCleanKeys start size=" + std::to_string(min_size));
        this->printUsage();
        long clean_size = 0;
        // min 8MB
        long x_size = BP_SIZE * 1024 * 2;
        this->share_mutex->lock();
        for (auto it = mymap->begin(); it != mymap->end();)
        {
            PValueType v;
            try
            {
                v = it->second;
            }
            catch (const std::exception &e)
            {
                this->slog->error("cleanKeys error key=", it->first.c_str());
                it++;
                continue;
            }
            clean_size += getDataSize(v.second);
            mymap->erase(it++);
            if (clean_size > x_size)
            {
                // todo should release more memory
                break;
            }
        }
        this->share_mutex->unlock();
        this->slog->debug("forceCleanKeys end");
        this->printUsage();
    }
    TDataType *get(std::string &key)
    {

        try
        {
            this->share_mutex->lock_sharable();
            auto it = mymap->find(ShmString(key.c_str(), managed_shm->get_segment_manager()));
            if (it != mymap->end())
            {
                TDataType *value = &it->second.second;
                this->share_mutex->unlock_sharable();
                long expireAt = it->second.first;
                long now_t = nnd::get_current_time_millis();
                if (now_t > expireAt)
                {
                    this->handleExpireKey(*it);
                    return nullptr;
                }

                return value;
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error get key " << key << e.what() << '\n';
            this->share_mutex->unlock_sharable();
        }
        return nullptr;
    }
    TDataType *get(const std::string &key)
    {
        std::string k = key;
        return this->get(k);
    }
    void remove(std::string &key)
    {
        this->share_mutex->lock();
        try
        {
            mymap->erase(ShmString(key.c_str(), managed_shm->get_segment_manager()));
        }
        catch (const std::exception &e)
        {
           this->slog->error("remove error key=", key);
        }
        this->share_mutex->unlock();
    }
    void destroy()
    {
        bip::shared_memory_object::remove(shm_name.c_str());
    }
    bool autoGrow(long min_size = 0)
    {
        // expensive operation , do not call frequently
        // this will copy all data to new memory ,so we make a large grow size
        long old_size = managed_shm->get_size();
        long grow_size = 0;
        if (old_size < BP_SIZE * 1024)
        {
            // <4MB auto grow x2
            grow_size = old_size * 2;
        }
        else if (old_size < BP_SIZE * 1024 * 50)
        {
            // 4MB-200MB auto grow 8MB
            grow_size = BP_SIZE * 1024 * 2;
        }
        else
        {
            // 400MB-4GB auto grow 20MB
            grow_size = BP_SIZE * 1024 * 5;
        }
        // grow_size min 40kb
        if (grow_size < BP_SIZE * 10)
        {
            grow_size = BP_SIZE * 10;
        }

        if (min_size > 0 && grow_size < min_size)
        {
            // use min_size to match insert data
            grow_size = min_size;
        }

        if (old_size + grow_size >= max_size)
        {
            this->slog->info("autoGrow out of memory max_size=" + std::to_string(max_size));
            this->printUsage();
            return false;
        }

        this->share_mutex->lock();
        try
        {
            this->slog->debug("autoGrow old_size_kb=", std::to_string(old_size / 1024));
            this->slog->debug("autoGrow grow_size_kb=", std::to_string(grow_size / 1024));
            int res = managed_shm->grow(shm_name.c_str(), grow_size);
            this->managed_shm = new bip::managed_shared_memory(bip::open_or_create, shm_name.c_str(), grow_size + old_size);
            this->mymap = managed_shm->find_or_construct<MyMap>("MyMap")(managed_shm->get_segment_manager());
        }
        catch (const std::exception &e)
        {
            std::cerr << 'Error auto grow ' << e.what() << '\n';
            std::cerr << 'old_size=' << old_size << " grow_size=" << grow_size << e.what() << '\n';
        }
        this->share_mutex->unlock();
        this->slog->debug("autoGrow new_size_kb=", std::to_string(this->managed_shm->get_size() / 1024));
        return true;
    }
    void autoShrink()
    {
        // expensive operation , do not call frequently
        this->share_mutex->lock();
        try
        {
            managed_shm->shrink_to_fit(shm_name.c_str());
        }
        catch (const std::exception &e)
        {
            std::cerr << "shrink_to_fit error " << e.what() << '\n';
        }
        this->share_mutex->unlock();
    }
    // get managed_shm size
    int get_size()
    {
        return managed_shm->get_size();
    }
    void printUsage()
    {
        this->slog->info("total_size=" + std::to_string(this->managed_shm->get_size()) + " free_size=" + std::to_string(this->managed_shm->get_free_memory()));
    }
    void setMaxAge(long age)
    {
        this->max_age = age;
    }
    void setLock(bool lock)
    {
        this->share_mutex->setEnable(lock);
    }
    void handleInsertLost(int insert_size)
    {
        this->cleanKeys();
        bool grow = this->autoGrow(insert_size * 2);
        const long free_size = this->managed_shm->get_free_memory();
        if (!grow)
        {
            this->slog->debug("out of memory try forceclean ");
            this->printUsage();
            this->forceCleanKeys(insert_size);
        }
    }
    void handleExpireKey(TPairType &pair)
    {
        long now_t = nnd::get_current_time_millis();
        if (now_t - last_clean_time > 5000)
        {
            this->cleanKeys();
        }
        else
        {
            mymap->erase(pair.first);
        }
    }
};

void printVt(TDataType &v)
{

    if (&v == nullptr)
    {
        std::cout << "null" << std::endl;
        return;
    }

    if (std::holds_alternative<bool>(v))
    {
        std::cout << std::get<bool>(v) << std::endl;
    }
    else if (std::holds_alternative<double>(v))
    {
        std::cout << std::get<double>(v) << std::endl;
    }
    else if (std::holds_alternative<std::string>(v))
    {
        std::cout << std::get<std::string>(v) << std::endl;
    }
    else if (std::holds_alternative<ShmString>(v))
    {
        std::cout << std::get<ShmString>(v) << std::endl;
    }
}

void printL(std::string k, double l)
{
    //
    std::cout << k << " = " << l << std::endl;
}

void insert30MB(BoostShareMap *bsm)
{
    std::string bs = "aluevaluevaluevaluevaluevaluevaluevaluevaluevaluevaluevaluevaluevaluevaluevaluevaluevaluevaluevaluevaluevaluevaluevaluevaluevaluevaluevaluevaluevaluevaluevaluevaluevaluevaluevaluevalue";
    for (int i = 0; i < 100000; i++)
    {
        std::string key = "key" + std::to_string(i);
        std::string value = bs + std::to_string(i);
        bsm->insert(key, std::string(value));
    }
}

int main()
{
    try
    {
        BoostShareMap *bsm = new BoostShareMap("Highscore", 1024, true);
        std::string vrrr = "aluevaluevaluevaluevaluevaluevaluevaluevaluevaluevaluevaluevaluevaluevaluevaluevaluevaluevaluevaluevaluevaluevaluevaluevaluevaluevaluevaluevaluevaluevaluevaluevaluevaluevaluevaluevalue";

        // insert
        // bsm->insert("k", 3.4);
        bsm->insert("k1", vrrr);
        bsm->insert("k2", vrrr + vrrr);
        bsm->insert("k3", vrrr + vrrr + vrrr);

        // get + print
        // printVt(*bsm->get("k1"));
        insert30MB(bsm);
        std::cout << " after usage " << std::endl;
        bsm->printUsage();
        sleep(1);
        std::cout << " after 1s " << std::endl;
        bsm->cleanKeys();
        bsm->printUsage();
        sleep(2);
        std::cout << " after 2s " << std::endl;
        bsm->cleanKeys();
        bsm->printUsage();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error main " << e.what() << '\n';
    }

    std::cout << "end" << std::endl;
}