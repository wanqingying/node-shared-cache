// include
#include <iostream>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/containers/pair.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/segment_manager.hpp>

namespace dtl = boost::container::dtl;

namespace bip = boost::interprocess;

typedef bip::allocator<char, bip::managed_shared_memory::segment_manager> CharAllocator;
typedef bip::basic_string<char, std::char_traits<char>, CharAllocator> ShmString;
typedef bip::allocator<ShmString, bip::managed_shared_memory::segment_manager> StringAllocator;
typedef std::pair<const ShmString, ShmString> ValueType;
typedef bip::allocator<ValueType, bip::managed_shared_memory::segment_manager> ShmAllocator;
typedef bip::map<ShmString, ShmString, std::less<ShmString>, ShmAllocator> MyMap;

class BoostShareMap
{
private:
    /* data */
    bip::managed_shared_memory *managed_shm;
    // bip::segment_manager
    // bip::segment_manager *sm;
    MyMap *mymap;
    std::string shm_name;
    int shm_size;

public:
    BoostShareMap(std::string name, int size, bool remove = false)
    {
        // print init
        std::cout << "BoostShareMap init" << std::endl;
        shm_name = name;
        shm_size = size;
        if (remove)
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
        // remove shared memory
        // bip::shared_memory_object::remove("Highscore");
    }
    void insert(std::string key, std::string value)
    {
        try
        {
            auto *sm = managed_shm->get_segment_manager();
            ValueType v1 = std::make_pair(ShmString(key.c_str(), sm), ShmString(value.c_str(), sm));
            // insert
            mymap->insert(v1);
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error inserting into shared memory: " << e.what() << std::endl;
        }
    }
    void print()
    {
        // print map
        for (auto it = mymap->begin(); it != mymap->end(); it++)
        {
            std::cout << it->first << " " << it->second << std::endl;
        }
    }
    std::string *get(std::string key)
    {
        auto it = mymap->find(ShmString(key.c_str(), managed_shm->get_segment_manager()));
        if (it != mymap->end())
        {
            const char *value = it->second.c_str();
            return new std::string(value);
        }
        // return null
        return nullptr;
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
    void autoGrow()
    {
        // auto grow
        managed_shm->grow(shm_name.c_str(), 1024);
    }
    void autoShrink()
    {
        try
        {
            managed_shm->shrink_to_fit(shm_name.c_str());
        }
        catch (const std::exception &e)
        {
            std::cerr<<"shrink_to_fit error " << e.what() << '\n';
        }
    }
    // get managed_shm size
    int size()
    {
        return managed_shm->get_size();
    }
};

int main2(int argc, char const *argv[])
{
    // // create a shared memory map by boost map
    // typedef bip::allocator<char, bip::managed_shared_memory::segment_manager> CharAllocator;
    // typedef bip::basic_string<char, std::char_traits<char>, CharAllocator> ShmString;
    // typedef bip::allocator<ShmString, bip::managed_shared_memory::segment_manager> StringAllocator;
    // typedef std::pair<const ShmString, ShmString> ValueType;
    // typedef bip::allocator<ValueType, bip::managed_shared_memory::segment_manager> ShmAllocator;
    // typedef bip::map<ShmString, ShmString, std::less<ShmString>, ShmAllocator> MyMap;

    // remove shared memory
    // bip::shared_memory_object::remove("Highscore");

    // // create shared memory
    // bip::managed_shared_memory managed_shm(bip::create_only, "Highscore", 1024);

    // // create allocator
    // // create map
    // MyMap *mymap = managed_shm.find_or_construct<MyMap>("MyMap")(managed_shm.get_segment_manager());

    // segment_manager *sm = managed_shm.get_segment_manager();

    // ValueType v1 = std::make_pair(ShmString("key1", sm), ShmString("value1", sm));
    // // insert
    // mymap->insert(v1);

    // // print map
    // for (auto it = mymap->begin(); it != mymap->end(); it++)
    // {
    //     std::cout << it->first << " " << it->second << std::endl;
    // }

    // return 0;
}

int main()
{
    BoostShareMap *bsm = new BoostShareMap("Highscore", 1024, true);
    bsm->insert("key1", "value1");
    bsm->insert("key2", "value2");

    // bsm->print();
    bsm->print();
    // print size
    std::cout << "size=" << bsm->size() << std::endl;
    bsm->autoGrow();
    // size
    std::cout << "size=" << bsm->size() << std::endl;
    bsm->autoShrink();
    // size
    std::cout << "size=" << bsm->size() << std::endl;
    // insert 
    bsm->insert("key3", "value3");

    std::cout << *bsm->get("key1") << std::endl;
}