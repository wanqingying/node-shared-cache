// include
#include <iostream>
#include <string>
#include <boost/container/map.hpp>
#include <functional>
#include <algorithm>

namespace bc = boost::container;

using namespace std;

int ucount = 1;

template <class T>
class PArray
{
private:
    /* data */
    T *p;
    int cap;
    int size;
    int ucid = ucount++;

public:
    PArray(int cap = 10)
    {
        // print
        std::cout << "PArray create " << ucid << std::endl;
        this->cap = cap;
        p = new T[cap];
        this->size = 0;
    };
    PArray(PArray &p)
    {
        // print
        std::cout << "PArray copy " << ucid << std::endl;
        this->cap = p.cap;
        this->p = new T[cap];
        for (int i = 0; i < cap; i++)
        {
            this->p[i] = p[i];
        }
        this->size = p.size;
    };
    ~PArray()
    {
        // print
        std::cout << "PArray destory " << ucid << std::endl;
        if (p != NULL)
        {
            delete[] p;
            p = NULL;
            this->size = 0;
        }
    }
    T &operator[](int i)
    {
        if (i < 0 || i >= cap)
        {
            std::cout << "out of range" << std::endl;
            exit(1);
        }

        return p[i];
    }
    PArray &operator=(PArray &p)
    {
        // print
        std::cout << "PArray operator = " << ucid << std::endl;
        if (this->p != NULL)
        {
            delete[] this->p;
            this->p = NULL;
        }
        this->cap = p.cap;
        this->p = new T[cap];
        for (int i = 0; i < cap; i++)
        {
            this->p[i] = p[i];
        }
        this->size = p.size;
        return *this;
    }
    int getSize()
    {
        return cap;
    }
    int push(const T &t)
    {
        if (size >= cap)
        {
            return -1;
        }
        p[size++] = t;
        return size;
    }
    T pop()
    {
        if (size <= 0)
        {
            return NULL;
        }
        return p[--size];
    }
    T *getArr()
    {
        return p;
    }
};

template <class T>
class Person
{
private:
    /* data */
    T age;
    std::string name;
    int akk = 0;

public:
    Person(string name, T age)
    {
        this->name = name;
        this->age = age;
    }
    Person()
    {
        this->name = "p";
        this->age = 0;
    }
    Person(const Person &p)
    {
        this->name = p.name;
        this->age = p.age;
    }
    ~Person() {}
    void say()
    {
        std::cout << "name=" << name << ", age=" << age << std::endl;
    }
    void operator()(int k)
    {
        this->akk += k;
        std::cout << "name=" << name << ", k=" << akk << std::endl;
    }
};


class MyVisitor
{
public:
    void operator()(bc::map<string, int>::value_type &p)
    {
        std::cout << p.first << "v=" << p.second << std::endl;
    }
};


// main
int main(int argc, char const *argv[])
{
    bc::map<string, int> m;
    m["a"] = 1;
    m["b"] = 2;
    m["g"] = 32;
    m["c"] = 4;
    m["1c"] = 41;

    for_each(m.begin(),m.end(), [](bc::map<string, int>::value_type &p) {
        std::cout << p.first << "v=" << p.second << std::endl;
    });
    // find(m.begin(),m.end(), [](bc::map<string, int>::value_type &p) {
    //     return p.first == "g";
    // });

    // iterator
    // for (bc::map<string, int>::iterator it = m.begin(); it != m.end(); it++)
    // {
    //     std::cout << it->first << "=" << it->second << std::endl;
    // }

    return 0;
}
