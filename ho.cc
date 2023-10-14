// write a hello world program using ios::cout and ios::cin.

#include <iostream> 
#include <string>


class Hello {
  public:
    Hello() {}
    ~Hello() {}
    void say();

    void print() {
      std::cout << "Hello World ho.cc" << std::endl;
    }
};

void Hello::say() {
  std::cout << "Hello World ho.cc" << std::endl;
}

int* intPtr() {
  int a=44;
  int* pa=&a;
  return pa;
}
  int *pg=NULL;
void change(int** p) {
  int a=55;
  *p=&a;
      std::cout << "p=" << *pg << std::endl;
}

void printStr(std::string &str) {
  // print *str
  std::cout << "*str=" << &str << std::endl;
  std::cout << "str=" << str << std::endl;
}



int main() {
  std::cout << "Hello World v66!" << std::endl;

  {
    int a=12;
    int *b=new int(13);
    // print &a 
    std::cout << "&a=" << &a << std::endl;
    // print &b
    std::cout << "&b=" << b << std::endl;
    delete b;

    pg=&a;
    std::cout << "p=" << *pg << std::endl;
    
  }
  std::string s1="printStr hello";
  // pring *s1
  std::cout << "*s1=" << &s1 << std::endl;

  printStr(s1);

  // print p
  std::cout << "p=" << *pg << std::endl;
  return 0;
}