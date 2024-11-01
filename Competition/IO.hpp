#ifndef MY_IO_HPP
#define MY_IO_HPP
/**
 * @author GuXinyang
 * @date 2024/10/31
 *
 * @brief namespace io 提供了快速的输入输出函数
 *
 * @brief namespace debug 提供了带颜色的调试输出
 * @note 在编译时加上 -DGXY_DEBUG 可以开启调试输出
 *
 * @brief namespace detime 提供了函数运行时间的统计
 * @note 在编译时加上 -DGXY_DEBUG 可以开启函数运行时间统计
 *
 * @example 使用示例
 * #include "IO.hpp"
 * void print()
 * {
 *     detime::Timer timer(__PRETTY_FUNCTION__);
 *     debug::cerr() << "Hello, World!\n";
 *     debug::cwarn() << "Hello, World!\n";
 *     debug::cinfo() << "Hello, World!\n";
 *     debug::cpurple() << "Hello, World!\n";
 *     debug::cwhite() << "Hello, World!\n";
 *     debug::cbold() << "Hello, World!\n";
 * }
 *
 * int main()
 * {
 *     print();
 *     detime::print_func_time();
 *     return 0;
 * }
 */

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

namespace io {
constexpr int MAXBUFFER = 1024 * 1024 * 8;
char ibuffer[MAXBUFFER], *iptr, obuffer[MAXBUFFER], *optr;
inline void start_reading()
{
    // 开始读取新的一行
    fgets(ibuffer, sizeof(ibuffer), stdin);
    iptr = ibuffer;
}

inline void start_writing()
{
    // 开始写入新的一行
    optr = obuffer;
}

inline int read_int()
{
    // 读取有符号整数
    char* nxt;
    int ret = strtol(iptr, &nxt, 10);
    iptr = nxt;
    return ret;
}

inline double read_double() noexcept
{
    char* nxt;
    double ret = strtod(iptr, &nxt);
    iptr = nxt;
    return ret;
}

inline void write_int(int val)
{
    char tmp[32], *now = tmp + 20;
    int length = 1;
    if (val < 0) {
        *optr++ = '-';
        val *= -1;
    }
    *now = ' ';
    do {
        *--now = '0' + val % 10;
        val /= 10;
        length += 1;
    } while (val > 0);
    memcpy(optr, now, length);
    optr += length;
}

inline void write_double(double val, int precision = 6)
{
    char tmp[32];
    sprintf(tmp, "%.*f ", precision, val);
    int length = strlen(tmp);
    memcpy(optr, tmp, length);
    optr += length;
}

inline void flush()
{
    if (optr != obuffer) {
        optr[-1] = '\n'; // 将最后一个空格改为换行符
    }
    fwrite(obuffer, 1, optr - obuffer, stdout);
    fflush(stdout);
}

inline void newline()
{
    if (optr != obuffer) {
        optr[-1] = '\n';
    }
}
} // namespace io

namespace debug {
inline std::ostream& endl(std::ostream& os)
{
    return os << std::endl;
}
#ifdef GXY_DEBUG
struct cerr {
    template <typename T>
    cerr& operator<<(const T& t)
    {
        std::cerr << "\033[31m" << t << "\033[0m";
        return *this;
    }

    // 特化处理std::ostream& (*pf)(std::ostream&)类型操作符
    cerr& operator<<(std::ostream& (*pf)(std::ostream&))
    {
        std::cerr << pf;
        return *this;
    }
};
struct cwarn {
    template <typename T>
    cwarn& operator<<(const T& t)
    {
        std::cerr << "\033[33m" << t << "\033[0m";
        return *this;
    }
    cwarn& operator<<(std::ostream& (*pf)(std::ostream&))
    {
        std::cerr << pf;
        return *this;
    }
};
struct cinfo {
    template <typename T>
    cinfo& operator<<(const T& t)
    {
        std::cerr << "\033[32m" << t << "\033[0m";
        return *this;
    }
    cinfo& operator<<(std::ostream& (*pf)(std::ostream&))
    {
        std::cerr << pf;
        return *this;
    }
};
struct cblue {
    template <typename T>
    cblue& operator<<(const T& t)
    {
        std::cerr << "\033[34m" << t << "\033[0m";
        return *this;
    }
    cblue& operator<<(std::ostream& (*pf)(std::ostream&))
    {
        std::cerr << pf;
        return *this;
    }
};
struct cpurple {
    template <typename T>
    cpurple& operator<<(const T& t)
    {
        std::cerr << "\033[35m" << t << "\033[0m";
        return *this;
    }
    cpurple& operator<<(std::ostream& (*pf)(std::ostream&))
    {
        std::cerr << pf;
        return *this;
    }
};
struct cwhite {
    template <typename T>
    cwhite& operator<<(const T& t)
    {
        std::cerr << "\033[37m" << t << "\033[0m";
        return *this;
    }
    cwhite& operator<<(std::ostream& (*pf)(std::ostream&))
    {
        std::cerr << pf;
        return *this;
    }
};
struct cbold {
    template <typename T>
    cbold& operator<<(const T& t)
    {
        std::cerr << "\033[1m" << t << "\033[0m";
        return *this;
    }
    cbold& operator<<(std::ostream& (*pf)(std::ostream&))
    {
        std::cerr << pf;
        return *this;
    }
};
#else
struct cerr {
    template <typename T>
    cerr& operator<<(const T& t)
    {
        return *this;
    }
    cerr& operator<<(std::ostream& (*pf)(std::ostream&))
    {
        return *this;
    }
};
struct cwarn {
    template <typename T>
    cwarn& operator<<(const T& t)
    {
        return *this;
    }
    cwarn& operator<<(std::ostream& (*pf)(std::ostream&))
    {
        return *this;
    }
};
struct cinfo {
    template <typename T>
    cinfo& operator<<(const T& t)
    {
        return *this;
    }
    cinfo& operator<<(std::ostream& (*pf)(std::ostream&))
    {
        return *this;
    }
};
struct cblue {
    template <typename T>
    cblue& operator<<(const T& t)
    {
        return *this;
    }
    cblue& operator<<(std::ostream& (*pf)(std::ostream&))
    {
        return *this;
    }
};

struct cpurple {
    template <typename T>
    cpurple& operator<<(const T& t)
    {
        return *this;
    }
    cpurple& operator<<(std::ostream& (*pf)(std::ostream&))
    {
        return *this;
    }
};

struct cwhite {
    template <typename T>
    cwhite& operator<<(const T& t)
    {
        return *this;
    }
    cwhite& operator<<(std::ostream& (*pf)(std::ostream&))
    {
        return *this;
    }
};

struct cbold {
    template <typename T>
    cbold& operator<<(const T& t)
    {
        return *this;
    }
    cbold& operator<<(std::ostream& (*pf)(std::ostream&))
    {
        return *this;
    }
};
#endif
} // namespace debug

#include <chrono>
#include <string>
#include <unordered_map>
namespace detime {
#ifdef GXY_DEBUG
/**
 * @brief 计算函数运行时间
 * @note 在每个函数的开头调用detime::Timer timer(__PRETTY_FUNCTION__);即可
 */
std::unordered_map<std::string, std::chrono::microseconds> func_time;
struct Timer {
    std::chrono::time_point<std::chrono::steady_clock> start;
    std::string func_name;
    Timer(const std::string& func_name)
        : start(std::chrono::steady_clock::now())
        , func_name(func_name)
    {
    }
    ~Timer()
    {
        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        func_time[func_name] += duration;
    }
};

void print_func_time()
{
    for (const auto& [func_name, duration] : func_time) {
        std::cerr << "Function: " << func_name << " takes " << duration.count() << " us" << std::endl;
    }
}
#else
struct Timer {
    Timer(const std::string&) { }
};
void print_func_time() { }
#endif
} // namespace functime

#endif // MY_IO_HPP