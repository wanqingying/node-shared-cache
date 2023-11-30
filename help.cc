#include <chrono>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <fstream>
#include <mach/mach.h>

namespace nnd
{
    using std::cout;
    using std::endl;
    using std::chrono::duration_cast;
    using std::chrono::milliseconds;
    using std::chrono::seconds;
    using std::chrono::system_clock;

    long print_memory_usage_mac()
    {
        task_basic_info info;
        mach_msg_type_number_t size = sizeof(info);
        kern_return_t kl = task_info(mach_task_self(), TASK_BASIC_INFO, (task_info_t)&info, &size);
        if (kl == KERN_SUCCESS)
        {
            std::cout << "Current memory usage: " << info.resident_size << " bytes" << std::endl;
        }
        else
        {
            std::cerr << "Failed to get memory info" << std::endl;
        }
        return info.resident_size;
    }
    size_t get_current_memory_usage()
    {
        std::ifstream status_file("/proc/self/status");
        std::string line;

        while (std::getline(status_file, line))
        {
            if (line.find("VmRSS") != std::string::npos)
            {
                size_t memory_usage = std::stoull(line.substr(line.find_last_of('\t'), line.find_last_of('k') - 1));
                return memory_usage;
            }
        }

        return 0;
    }

    long get_current_time_millis()
    {
        auto millisec_since_epoch = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        return millisec_since_epoch;
    }

    enum ELevel
    {
        DEBUG = 1,
        INFO,
        ERROR
    };

    class ShmLog
    {
    private:
        /* data */

        std::string prefix = "";
        ELevel level = DEBUG;

    public:
        ShmLog(/* args */)
        {
        }
        ~ShmLog() {}

        void setLevel(ELevel level)
        {
            this->level = level;
        }

        void info(std::string msg)
        {
            if (this->level > INFO)
            {
                return;
            }

            std::time_t t = std::time(nullptr);
            std::tm tm = *std::localtime(&t);
            std::cout << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << " [INFO] "
                      << msg << std::endl;
        }
        void info(const char *msg)
        {
            if (this->level > INFO)
            {
                return;
            }
            std::time_t t = std::time(nullptr);
            std::tm tm = *std::localtime(&t);
            std::cout << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << " [INFO] "
                      << msg << std::endl;
        }
        void debug(const char *msg)
        {
            if (this->level > DEBUG)
            {
                return;
            }
            std::time_t t = std::time(nullptr);
            std::tm tm = *std::localtime(&t);
            std::cout << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << " [DEBUG] "
                      << msg << std::endl;
        }
        void debug(std::string msg)
        {
            if (this->level > DEBUG)
            {
                return;
            }
            std::time_t t = std::time(nullptr);
            std::tm tm = *std::localtime(&t);
            std::cout << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << " [DEBUG] "
                      << msg << std::endl;
        }
        void debug(const char *msg, std::string msg2)
        {
            if (this->level > DEBUG)
            {
                return;
            }
            std::time_t t = std::time(nullptr);
            std::tm tm = *std::localtime(&t);
            std::cout << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << " [DEBUG] "
                      << msg << msg2 << std::endl;
        }
        void error(const char *msg, std::string msg2)
        {
            if (this->level > ERROR)
            {
                return;
            }
            std::time_t t = std::time(nullptr);
            std::tm tm = *std::localtime(&t);
            std::cout << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << " [ERROR] "
                      << msg << msg2 << std::endl;
        }
        void error(const char *msg)
        {
            if (this->level > ERROR)
            {
                return;
            }
            std::string msg2 = "";
            return this->error(msg, msg2);
        }
    };

}
