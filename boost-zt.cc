#include <iostream>
#include <utility>
#include <thread>
#include <chrono>
#include <functional>
#include <atomic>
#include <random>
#include <string>
#include "boost-share-cache.cc"

void runWrite(BoostShareCache *bsc)
{
    stdstring v1 = "v1";
    stdstring k1 = "k1";
    bsc->insert(k1, v1);
    std::string *exp = new std::string(5000, 'a');
    for (size_t i = 0; i < 6; i++)
    {
        stdstring ki = "key" + std::to_string(i);
        bsc->insert(ki, *exp);
    }
}

int main(int argc, char *argv[])
{
    std::cout << "start" << std::endl;
    bool isMain = true;
    int pid = fork();
    if (pid == -1)
    {
        std::cerr << "fork failed" << std::endl;
    }
    else if (pid == 0)
    {
        isMain = false;
    }
    else
    {
        isMain = true;
    }
    BoostShareCache *bsc = new BoostShareCache("HightScoreV4", 1024 * 4, isMain);
    runWrite(bsc);

    if (isMain)
    {
        
        // wait pid
        int status;
        waitpid(pid, &status, 0);

    }

    return 0;
}
