#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/list.hpp>
#include <boost/interprocess/containers/flat_map.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <iostream>
#include "help.cc"
namespace bip = boost::interprocess;

namespace slc
{
    typedef std::string stdstring;
    typedef bip::allocator<char, bip::managed_shared_memory::segment_manager> CharAllocator;
    typedef bip::basic_string<char, std::char_traits<char>, CharAllocator> ShmString;

    template <typename Value, typename KeyItor>
    struct MapValue
    {
        Value value;
        KeyItor itor;
        long expire_at;
    };

    template <typename Key, typename Value>
    class SharedLRUCache
    {
    public:
        typedef bip::allocator<Key, bip::managed_shared_memory::segment_manager> KeyAllocator;
        typedef bip::allocator<Value, bip::managed_shared_memory::segment_manager> ValueAllocator;
        typedef bip::list<Key, KeyAllocator> KeyList;
        typedef typename KeyList::iterator list_iterator;
        typedef MapValue<Value, list_iterator> MapValueType;
        typedef std::pair<Key, MapValueType> TPairType;
        typedef bip::allocator<std::pair<Key, MapValueType>, bip::managed_shared_memory::segment_manager> MapValueAllocator;
        typedef bip::flat_map<Key, MapValueType, std::less<Key>, MapValueAllocator> FlatMap;

        FlatMap *map;
        KeyList *keys;
        size_t capacity;
        bip::managed_shared_memory segment;

    public:
        SharedLRUCache(std::string name, size_t capacity)
            : segment(bip::create_only, name.c_str(), 4096)
        {
            auto segmentManager = segment.get_segment_manager();
            this->map = segment.find_or_construct<FlatMap>("cache")(segmentManager);
            std::cout << map->max_size() << std::endl;

            this->keys = segment.find_or_construct<KeyList>("keys")(segmentManager);

            this->capacity = capacity;
        }

        Value get(Key key)
        {
            auto it = map->find(key);
            if (it == map->end())
            {
                return -1; // or other "not found" value
            }
            // keys->splice(keys->begin(), *keys, it->second.second);
            // return it->second.first;
            keys->splice(keys->begin(), *keys, it.second.itor);
            return it.second.value;
        }

        // remove
        void remove(Key key)
        {
            auto it = map->find(key);
            if (it == map->end())
            {
                return;
            }
            keys->erase(it->second.itor);
            map->erase(it);
        }

        void put(Key key, Value value)
        {
            auto it = this->map->find(key);

            if (it != map->end())
            {
                keys->splice(keys->begin(), *keys, it->second.itor);
                it->second.value = value;
                return;
            }
            if (map->size() == capacity)
            {
                Key last = keys->back();
                keys->pop_back();
                map->erase(last);
            }

            keys->push_front(key);
            long now_time = nnd::get_current_time_millis();
            map->insert(std::make_pair(key, MapValueType{value, keys->begin(), now_time}));
        }
        ShmString mstring(stdstring &str)
        {
            return ShmString(str.c_str(), segment.get_segment_manager());
        }
    };
}

int main()
{
    using namespace slc;
    bip::shared_memory_object::remove("MySharedMemory");
    auto cache = SharedLRUCache<ShmString, ShmString>("MySharedMemory", 3);
    stdstring key1 = "key1";
    stdstring value1 = "value1";
    cache.put(cache.mstring(key1), cache.mstring(value1));

    // print
    // for (auto it = cache.keys->begin(); it != cache.keys->end(); it++)
    // {
    //     // print key=v
    //     std::cout << *it << "=" << cache.get(*it) << std::endl;
    // }

    // print cache.cache

    for (auto it = cache.map->begin(); it != cache.map->end(); it++) // tag3
    {
        std::cout << it->first << "=" << it->second.value << std::endl;
    }

    // Use the cache...
}