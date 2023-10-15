// include
#include <iostream>

class Base
{
private:
    /* data */
    int b=3;
public:
    Base(/* args */){

    }
    ~Base(){

    }
};

class Extend1:public Base
{
public:
    Extend1(/* args */){
        std::cout << "Extend1" << std::endl;
    }
    // copy
    Extend1(const Extend1& e){
        std::cout << "copy" << std::endl;
    }
    ~Extend1(){

    }
private:
    /* data */
    int a=3;
    int b=6;
};

template <typename T>;
void print(T t){
    std::cout << t << std::endl;
}


// main 
int main(int argc, char const *argv[])
{
    Extend1 e1;
    // print *e1
    Base b1=e1;
    // Extend1 e2=e1;
    std::cout << "e1=" << &e1 << std::endl;
    std::cout << "b1=" << &b1 << std::endl;
    // std::cout << "e2=" << &e2 << std::endl;
    return 0;
}





