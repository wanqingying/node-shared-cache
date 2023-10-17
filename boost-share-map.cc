// include
#include <iostream>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/containers/pair.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/segment_manager.hpp>
#include <boost/interprocess/sync/named_sharable_mutex.hpp>
#include <boost/interprocess/sync/named_condition.hpp>

namespace dtl = boost::container::dtl;

namespace bip = boost::interprocess;

typedef bip::allocator<char, bip::managed_shared_memory::segment_manager> CharAllocator;
typedef bip::basic_string<char, std::char_traits<char>, CharAllocator> ShmString;
typedef bip::allocator<ShmString, bip::managed_shared_memory::segment_manager> StringAllocator;
typedef std::pair<const ShmString, ShmString> ValueType;
typedef bip::allocator<ValueType, bip::managed_shared_memory::segment_manager> ShmAllocator;
typedef bip::map<ShmString, ShmString, std::less<ShmString>, ShmAllocator> MyMap;

// memory page size

class BoostShareMap
{
    // linux memory page size
    static const int BP_SIZE = 1024 * 4;
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
    int max_size = 1024 * 1024;
    bip::named_sharable_mutex *mutex;
    bool isMaster = false;

public:
    BoostShareMap(std::string name, int size, bool isMaster = false)
    {
        // print init
        std::cout << "BoostShareMap init" << std::endl;
        std::string mtx_name = shm_name + "_mtx";
        shm_name = name;
        shm_size = size;
        if (isMaster)
        {
            bip::shared_memory_object::remove(shm_name.c_str());
            bip::named_sharable_mutex::remove(mtx_name.c_str());
        }
        try
        {
            managed_shm = new bip::managed_shared_memory(bip::open_or_create, name.c_str(), size);
            mymap = managed_shm->find_or_construct<MyMap>("MyMap")(managed_shm->get_segment_manager());
            mutex = new bip::named_sharable_mutex(bip::open_or_create, mtx_name.c_str());
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
            // remove mutex
            std::string mtx_name = shm_name + "_mtx";
            bip::named_sharable_mutex::remove(mtx_name.c_str());
        }
    }

    void insert(std::string &key, std::string &value, bool retry = true)
    {
        // key.size +value.size
        int pairSize = key.size() + value.size();
        try
        {
            // lock
            mutex->lock();
            auto *sm = managed_shm->get_segment_manager();
            ValueType v1 = std::make_pair(ShmString(key.c_str(), sm), ShmString(value.c_str(), sm));
            auto pair = mymap->insert(v1);
            if (!pair.second)
            {
                // exist key replace
                mymap->erase(pair.first);
                mymap->insert(v1);
            }
            mutex->unlock();
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error inserting into shared memory: " << e.what() << std::endl;
            this->autoGrow(pairSize);
            // insert again
            mutex->unlock();
            if (retry)
            {
                this->insert(key, value, false);
            }
        }
    }
    void insert(const std::string &key, const std::string &value)
    {
        // this.insert
        std::string k = key;
        std::string v = value;
        this->insert(k, v);
    }
    void print()
    {
        // print map
        for (auto it = mymap->begin(); it != mymap->end(); it++)
        {
            std::cout << it->first << " " << it->second << std::endl;
        }
    }
    std::string *get(std::string &key)
    {
        try
        {
            // share lock
            mutex->lock_sharable();
            auto it = mymap->find(ShmString(key.c_str(), managed_shm->get_segment_manager()));
            if (it != mymap->end())
            {
                const char *value = it->second.c_str();
                mutex->unlock_sharable();
                return new std::string(value);
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error get key " << key << e.what() << '\n';
            mutex->unlock_sharable();
        }
        return nullptr;
    }
    std::string *get(const std::string &key)
    {
        std::string k = key;
        return this->get(k);
    }
    void remove(std::string key)
    {
        mymap->erase(ShmString(key.c_str(), managed_shm->get_segment_manager()));
    }
    void destroy()
    {
        // remove shared memory
        bip::shared_memory_object::remove(shm_name.c_str());
    }
    void autoGrow(int min_size = 0)
    {
        int old_size = managed_shm->get_size();
        int grow_size = 0;
        // print old_size max_size
        std::cout << "old_size = " << old_size << ", max_size = " << max_size << std::endl;
        // print page_size
        std::cout << "page_size = " << BP_SIZE << std::endl;
        if (old_size >= max_size)
        {
            std::cout << "memory reach max size" << std::endl;
            return;
        }
        if (old_size < BP_SIZE * 1024)
        {
            // <4MB auto grow x2
            grow_size = old_size * 2;
        }
        else if (old_size < BP_SIZE * 1024 * 100)
        {
            // 4MB-400MB auto grow 400KB
            grow_size = old_size + BP_SIZE * 100;
        }
        else
        {
            // 400MB-4GB auto grow 4MB
            grow_size = old_size + BP_SIZE * 1024;
        }
        // grow_size min 4kb
        if (grow_size < BP_SIZE)
        {
            grow_size = BP_SIZE;
        }

        if (min_size > 0 && grow_size < min_size)
        {
            grow_size = min_size;
        }

        // print
        std::cout << "auto grow  size =  " << grow_size << std::endl;
        // auto grow
        managed_shm->grow(shm_name.c_str(), grow_size);
    }
    void autoShrink()
    {
        try
        {
            managed_shm->shrink_to_fit(shm_name.c_str());
        }
        catch (const std::exception &e)
        {
            std::cerr << "shrink_to_fit error " << e.what() << '\n';
        }
    }
    // get managed_shm size
    int size()
    {
        return managed_shm->get_size();
    }
};


int main()
{
    BoostShareMap *bsm = new BoostShareMap("Highscore", 2048, true);
    bsm->insert("key1", "value1");
    bsm->insert("key2", "value2");
    // print
    std::cout << "inser large string" << std::endl;

    // bsm->print();
    bsm->print();
    // bsm->autoShrink();
    // bsm->insert("key1", "vxxx");
    bsm->insert("key1", std::string(2048 * 2, 'a'));
    std::string k2 = "key1";

    std::cout << *bsm->get(k2) << std::endl;
    // print size
    std::cout << "size = " << bsm->size() << std::endl;
}