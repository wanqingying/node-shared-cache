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

// todo test for this
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
    else if (auto str_ptr = std::get_if<std::string>(&v))
    {
        return str_ptr->size();
    }
    else if (auto shm_str_ptr = std::get_if<ShmString>(&v))
    {
        return shm_str_ptr->size();
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
    void destroy()
    {
        bip::named_sharable_mutex::remove(name.c_str());
    }
};

class BoostShareCache
{
    // linux memory page size
    static const long BP_SIZE = 1024 * 4;
    // max alloc memory size 4G
    // static const int FULL_MAX_SIZE = 1024 * 4 * 1024 * 1024;

public:
    /* data */
    bip::managed_shared_memory *managed_shm;
    bip::managed_shared_memory *shm_vn;
    // bip::segment_manager
    // bip::segment_manager *sm;
    MyMap *mymap;
    long *last_clean_time;
    int *grow_tag;
    int *version;
    int version_count = 0;
    int grow_count = 0;

    std::string shm_name;
    long shm_size;
    // default 800MB
    long max_size = BP_SIZE * 1024 * 200;
    BoostNamedShareMutex *share_mutex;
    // for nodejs fork
    bool isMaster = false;
    // milliseconds
    long max_age = 2000;
    bool renew = true;

public:
    nnd::ShmLog *slog = new nnd::ShmLog();

private:
    void getResource(long size)
    {
        this->getResource(size, 0, nnd::get_current_time_millis());
    }
    void setVersionPlus()
    {
        *this->version = *this->version + 1;
        this->version_count = *this->version;
    }
    void getResource(long size, int tag, long last_c_time, int version = 1)
    {
        std::string mtx_name = shm_name + "_mtx";
        std::string ver_name = shm_name + "_ver";
        this->managed_shm = new bip::managed_shared_memory(bip::open_or_create, this->shm_name.c_str(), size);
        this->shm_vn = new bip::managed_shared_memory(bip::open_or_create, ver_name.c_str(), 2048);
        this->mymap = managed_shm->find_or_construct<MyMap>("MyMap")(managed_shm->get_segment_manager());
        this->last_clean_time = shm_vn->find_or_construct<long>("last_clean_time")(last_c_time);
        this->grow_tag = shm_vn->find_or_construct<int>("grow_tag")(0);
        this->version = this->shm_vn->find_or_construct<int>("version")(version + 1);
        this->version_count = *this->version;
    }
    void getResource()
    {
        this->getResource(this->shm_size, 0, nnd::get_current_time_millis());
    }
    long getLastCleanTime()
    {
        return *this->last_clean_time;
    }
    void setLastCleanTime()
    {
        *this->last_clean_time = nnd::get_current_time_millis();
    }
    bool isGrowTagNotOk()
    {
        // can be replaced by mutex
        return *this->grow_tag == 1;
    }
    void setGrowTagEnter()
    {
        *this->grow_tag = 1;
    }
    void setGrowTagOut()
    {
        *this->grow_tag = 0;
    }

public:
    BoostShareCache(std::string name, int size, bool renew = false)
    {
        // print init
        this->slog->info("BoostShareCache init");
        std::string mtx_name = shm_name + "_mtx";
        std::string ver_name = shm_name + "_ver";
        shm_name = name;
        shm_size = size;
        if (renew)
        {
            bip::shared_memory_object::remove(shm_name.c_str());
            bip::named_sharable_mutex::remove(ver_name.c_str());
        }
        try
        {
            this->getResource();
            this->share_mutex = new BoostNamedShareMutex(mtx_name, renew);
        }
        catch (const std::exception &e)
        {
            this->slog->error("BoostShareCache init error ", e.what());
        }
    }
    ~BoostShareCache()
    {
        if (isMaster)
        {
            // remove shared memory
            // bip::shared_memory_object::remove(shm_name.c_str());
        }
    }

    void checkVersion()
    {
        if (this->version_count != *this->version)
        {
            this->slog->debug("version changed reset resource");
            this->getResource();
        }
    }
    void insert(std::string &key, TDataType &value, long maxAge, bool retry = true)
    {
        try
        {
            this->share_mutex->lock();
            this->checkVersion();
            auto *sm = managed_shm->get_segment_manager();
            PValueType pv;
            long expire_at = nnd::get_current_time_millis() + maxAge;
            if (auto bool_ptr = std::get_if<bool>(&value))
            {
                pv = std::make_pair(expire_at, *bool_ptr);
            }
            else if (auto double_ptr = std::get_if<double>(&value))
            {
                pv = std::make_pair(expire_at, *double_ptr);
            }
            else if (auto str_ptr = std::get_if<std::string>(&value))
            {
                pv = std::make_pair(expire_at, ShmString(str_ptr->c_str(), sm));
            }
            else if (auto shm_str_ptr = std::get_if<ShmString>(&value))
            {
                pv = std::make_pair(expire_at, *shm_str_ptr);
            }
            else
            {
                this->slog->error("insert error unsupport value type ", typeid(value).name());
                this->share_mutex->unlock();
                return;
            }
            ShmString k(key.c_str(), sm);
            TPairType pair = std::make_pair(k, pv);
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

            this->slog->debug("handleInsertLost key=" + key + " " + e.what());
            int pairSize = key.size() + getDataSize(value);
            this->share_mutex->unlock();
            this->handleInsertLost(pairSize);
            if (retry)
            {
                this->slog->debug("retry insert key=", key);
                this->insert(key, value, false, maxAge);
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
        this->insert(k, value, true);
    }
    void insert(std::string key, double value)
    {
        TDataType v = buildType(value);
        this->insert(key, v, this->max_age, true);
    }
    void insert(std::string key, bool value)
    {
        TDataType v = buildType(value);
        this->insert(key, v, this->max_age, true);
    }
    void insert(std::string key, std::string value)
    {
        TDataType v = buildType(value);
        this->insert(key, v, this->max_age, true);
    }
    void insert(std::string key, std::string value, long maxAge)
    {
        TDataType v = buildType(value);
        this->insert(key, v, maxAge, true);
    }

    void cleanKeys()
    {
        this->share_mutex->lock();
        long now_t = nnd::get_current_time_millis();
        if (now_t - *this->last_clean_time < 2000)
        {
            this->slog->debug("cleanKeys skip");
            this->share_mutex->unlock();
            return;
        }
        this->setLastCleanTime();

        this->slog->debug("cleanKeys start");
        this->printUsage();

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
        this->slog->debug("cleanKeys end");
        this->share_mutex->unlock();
        this->printUsage();
        this->autoShrink();
    }
    void forceCleanKeys(long min_size)
    {
        this->slog->debug("forceCleanKeys start size=" + std::to_string(min_size));
        this->printUsage();
        this->share_mutex->lock();
        this->checkVersion();
        long free_size = this->managed_shm->get_free_memory();
        long total_size = this->managed_shm->get_size();
        if (free_size > BP_SIZE * 1024 && total_size > min_size * 2)
        {
            this->slog->debug("forceCleanKeys skip");
            this->share_mutex->unlock();
            return;
        }

        // min 8MB
        long x_size = (long)(total_size / 3);
        long clean_size = 0;

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
        this->slog->info("forceCleanKeys end clean_size=" + std::to_string(clean_size));
        this->printUsage();
    }
    TDataType *get(std::string &key)
    {
        try
        {

            this->share_mutex->lock_sharable();
            this->checkVersion();
            auto it = mymap->find(ShmString(key.c_str(), managed_shm->get_segment_manager()));
            if (it != mymap->end())
            {
                TDataType *value = &it->second.second;

                long expireAt = it->second.first;
                long now_t = nnd::get_current_time_millis();
                if (now_t > expireAt)
                {
                    this->share_mutex->unlock_sharable();
                    this->handleExpireKey(*it);
                    return nullptr;
                }
                this->share_mutex->unlock_sharable();
                return value;
            }
            else
            {
                this->share_mutex->unlock_sharable();
                return nullptr;
            }
        }
        catch (const std::exception &e)
        {
            this->slog->error("get error key=", key);
            this->share_mutex->unlock_sharable();
            return nullptr;
        }
    }
    TDataType *get(const std::string &key)
    {
        std::string k = key;
        return this->get(k);
    }
    void remove(std::string key)
    {
        this->share_mutex->lock();
        this->checkVersion();
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
        // this should be called by master process only
        bip::shared_memory_object::remove(shm_name.c_str());
        bip::shared_memory_object::remove((shm_name + "_ver").c_str());
        this->share_mutex->destroy();
    }
    bool autoGrow(long min_size = 0)
    {
        // should check mechine memory but not implement,  set max_size careful
        this->share_mutex->lock();
        // this->checkVersion();
        if (this->isGrowTagNotOk())
        {
            this->slog->debug("autoGrow skip");
            this->share_mutex->unlock();
            return true;
        }
        else
        {
            this->slog->debug("autoGrow start");
            this->setGrowTagEnter();
        }
        // auto grow can not be multi process call
        // because lock is not shareable
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
            this->setGrowTagOut();
            this->share_mutex->unlock();
            return false;
        }

        try
        {
            this->slog->debug("autoGrow old_size_kb=", std::to_string(old_size / 1024));
            this->slog->debug("autoGrow grow_size_kb=", std::to_string(grow_size / 1024));
            int tag = *this->grow_tag;
            long last_c_time = *this->last_clean_time;
            managed_shm->grow(shm_name.c_str(), grow_size);
            this->getResource(old_size + grow_size, tag, last_c_time);
            this->setVersionPlus();
        }
        catch (const std::exception &e)
        {
            this->slog->info("autoGrow error " + std::string(e.what()));
        }
        this->setGrowTagOut();
        this->share_mutex->unlock();
        this->grow_count++;
        this->slog->debug("autoGrow new_size_kb=", std::to_string(this->managed_shm->get_size() / 1024));
        return true;
    }
    void autoShrink()
    {

        // must lock before isShrink check
        // avoid multi process shrink at same time
        this->share_mutex->lock();
        // this->checkVersion();
        long total_size = this->managed_shm->get_size();
        long free_size = this->managed_shm->get_free_memory();
        bool isShrink = free_size > total_size / 2 && total_size > BP_SIZE * 1024 * 10;
        if (!isShrink)
        {
            this->share_mutex->unlock();
            this->slog->debug("autoShrink skip");
            return;
        }

        this->slog->debug("autoShrink start");
        this->printUsage();
        try
        {
            int tag = *this->grow_tag;
            long last_c_time = *this->last_clean_time;
            managed_shm->shrink_to_fit(shm_name.c_str());
            this->getResource(managed_shm->get_size(), tag, last_c_time);
            this->setVersionPlus();
        }
        catch (const std::exception &e)
        {
            this->slog->error("autoShrink error ", e.what());
        }
        this->share_mutex->unlock();
        this->slog->debug("autoShrink end");
        this->printUsage();
    }
    // get managed_shm size
    int get_size()
    {
        return managed_shm->get_size();
    }
    void printUsage()
    {
        this->slog->debug("total_size=" + std::to_string(this->managed_shm->get_size()) + " free_size=" + std::to_string(this->managed_shm->get_free_memory()));
    }
    void setMaxAge(long age)
    {
        this->max_age = age;
    }
    void setMaxSize(long size)
    {
        this->max_size = size;
    }
    void setLock(bool lock)
    {
        this->share_mutex->setEnable(lock);
    }
    void setLogLevel(nnd::ELevel level)
    {
        this->slog->setLevel(level);
    }
    void handleInsertLost(int insert_size)
    {
        this->cleanKeys();

        bool grow = this->autoGrow(insert_size * 2);
        this->printUsage();
        if (!grow)
        {
            this->slog->debug("out of memory try forceclean ");
            this->forceCleanKeys(insert_size);
        }
    }
    void handleExpireKey(TPairType &pair)
    {
        this->share_mutex->lock();
        try
        {
            mymap->erase(pair.first);
        }
        catch (const std::exception &e)
        {
            this->slog->error("handleExpireKey erase error key=", pair.first.c_str());
        }
        this->share_mutex->unlock();
    }
    void printGrowCont()
    {
        this->slog->info("grow_count=" + std::to_string(this->grow_count));
    }
};
