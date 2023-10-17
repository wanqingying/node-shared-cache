#include <chrono>
#include <iostream>
#include <sys/time.h>
#include <iomanip>
#include <ctime>

namespace nnd
{
    using std::cout;
    using std::endl;
    using std::chrono::duration_cast;
    using std::chrono::milliseconds;
    using std::chrono::seconds;
    using std::chrono::system_clock;
    long get_current_time_millis()
    {
        auto millisec_since_epoch = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        return millisec_since_epoch;
    }

    class ShmLog
    {
    private:
        /* data */

        std::string prefix = "";

    public:
        ShmLog(/* args */)
        {
        }
        ~ShmLog() {}

        void info(std::string msg)
        {
            std::time_t t = std::time(nullptr);
            std::tm tm = *std::localtime(&t);
            std::cout << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << " [INFO] "
                      << msg << std::endl;
        }
        void info(const char *msg)
        {
            std::time_t t = std::time(nullptr);
            std::tm tm = *std::localtime(&t);
            std::cout << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << " [INFO] "
                      << msg << std::endl;
        }
        void debug(const char *msg)
        {
            std::time_t t = std::time(nullptr);
            std::tm tm = *std::localtime(&t);
            std::cout << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << " [DEBUG] "
                      << msg << std::endl;
        }
        void debug(std::string msg)
        {
            std::time_t t = std::time(nullptr);
            std::tm tm = *std::localtime(&t);
            std::cout << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << " [DEBUG] "
                      << msg << std::endl;
        }
        void debug(const char *msg, std::string msg2)
        {
            std::time_t t = std::time(nullptr);
            std::tm tm = *std::localtime(&t);
            std::cout << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << " [DEBUG] "
                      << msg << msg2 << std::endl;
        }
        void error(const char *msg, std::string msg2)
        {
            std::time_t t = std::time(nullptr);
            std::tm tm = *std::localtime(&t);
            std::cout << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << " [ERROR] "
                      << msg << msg2 << std::endl;
        }
        void error(const char *msg)
        {
            std::string msg2 = "";
            return this->error(msg, msg2);
        }
    };

}

// void printx(double d)
// {
//     // print d
//     std::cout << d << std::endl;
// }

// int main()
// {

//     // print time
//     long t1 = nnd::get_current_time_millis();
//     // printx t1
//     printx(t1);

//     return EXIT_SUCCESS;
// }