// write a hello world program using ios::cout and ios::cin.

#include <iostream>
#include <string>
#include <boost/container/map.hpp>

using namespace std;

int ucount = 1;

class Hello
{
  friend ostream &operator<<(ostream &_cout, const Hello &hello)
  {
    _cout << "a=" << hello.a << ", b=" << hello.b << ", name=" << hello.name;
    return _cout;
  }

public:
  // ++ operator
  Hello operator+(Hello &h)
  {
    Hello hello;
    hello.a = this->a + h.a;
    hello.b = this->b + h.b;
    return hello;
  }

  Hello &operator-(auto)
  {
    // print not support
    std::cout << "operator- not support" << std::endl;
    return *this;
  }

  void operator+(int a)
  {
    this->a += a;
    this->b += a;
  }

  // h++
  Hello& operator++(int)
  {
    this->a++;
    this->b++;
    return *this;
  }
  Hello operator++()
  {
    // print 
    std::cout << "operator++ 1" << std::endl;
    Hello tmp=*this;
    std::cout << "operator++ 2" << std::endl;
    this->a++;
    this->b++;
    return tmp;
  }

  void operator()(string arg)
  {
    std::cout << "name=" << name << ", arg=" << arg << std::endl;
  }

public:
  Hello()
  {
    a = 1;
    b = 2;
    // set name with ++count
    name = "hello_v_" + to_string(ucount++);
  }
  // constructor a b
  Hello(int a, int b)
  {
    this->a = a;
    this->b = b;
    // set name with ++count
    name = "hello_v_" + to_string(ucount++);
  }
  // copy constructor
  Hello(const Hello &h)
  {
    // print 
    std::cout << "copy constructor" << std::endl;
    this->a = h.a;
    this->b = h.b;
    name = "hello_v_" + to_string(ucount++);
  }
  ~Hello() {}
  void say()
  {
    std::cout << "Hello World ho.cc" << std::endl;
  }

  void print()
  {
    std::cout << "Hello World ho.cc" << std::endl;
  }

private:
  int a;
  int b;
  string name = "h_v_0";
};

int *intPtr()
{
  int a = 44;
  int *pa = &a;
  return pa;
}
int *pg = NULL;
void change(int **p)
{
  int a = 55;
  *p = &a;
  std::cout << "p=" << *pg << std::endl;
}

void printStr(std::string &str)
{
  // print *str
  std::cout << "*str=" << &str << std::endl;
  std::cout << "str=" << str << std::endl;
}

int main()
{
  std::cout << "Hello World v66!" << std::endl;

  Hello oh;
  std::cout << oh << std::endl;
  oh++;
  std::cout << oh << std::endl;
  Hello oh2= ++oh;
  std::cout << oh2 << std::endl;
  std::cout << oh << std::endl;
  oh("vvvvvvv");
  return 0;
}