#include <napi.h>
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
    bip::managed_shared_memory *managed_shm;
    MyMap *mymap;
    std::string shm_name;
    int shm_size;

public:
    BoostShareMap(std::string name, int size, bool remove = false)
    {
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
        bip::shared_memory_object::remove(shm_name.c_str());
    }
    void insert(std::string key, std::string value)
    {
        try
        {
            auto *sm = managed_shm->get_segment_manager();
            ValueType v1 = std::make_pair(ShmString(key.c_str(), sm), ShmString(value.c_str(), sm));
            mymap->insert(v1);
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error inserting into shared memory: " << e.what() << std::endl;
        }
    }
    void print()
    {
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
        return nullptr;
    }
    void remove(std::string key)
    {
        mymap->erase(ShmString(key.c_str(), managed_shm->get_segment_manager()));
    }
    void destroy()
    {
        bip::shared_memory_object::remove(shm_name.c_str());
    }
    void autoGrow()
    {
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
    int size()
    {
        return managed_shm->get_size();
    }
};

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function func = DefineClass(env, "BoostShareMap", {
        InstanceMethod("insert", &BoostShareMap::insert),
        InstanceMethod("print", &BoostShareMap::print),
        InstanceMethod("get", &BoostShareMap::get),
        InstanceMethod("remove", &BoostShareMap::remove),
        InstanceMethod("destroy", &BoostShareMap::destroy),
        InstanceMethod("autoGrow", &BoostShareMap::autoGrow),
        InstanceMethod("autoShrink", &BoostShareMap::autoShrink),
        InstanceMethod("size", &BoostShareMap::size),
        InstanceMethod("print2", &BoostShareMap::print),
    });

    Napi::Object obj = func.New({Napi::String::New(env, "my_shm"), Napi::Number::New(env, 1024)});
    exports.Set(Napi::String::New(env, "BoostShareMap"), func);

    return exports;
}

NODE_API_MODULE(addon, Init)
