/*
 * Copyright (c) 2005-2024
 */
/*
 * Mike Mirzayanov
 *
 * This material is provided "as is", with absolutely no warranty expressed
 * or implied. Any use is at your own risk.
 *
 * Permission to use or copy this software for any purpose is hereby granted
 * without fee, provided the above notices are retained on all copies.
 * Permission to modify the code and to distribute modified code is granted,
 * provided the above notices are retained, and a notice that the code was
 * modified is included with the above copyright notice.
 *
 */
#include "IO.hpp"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <set>
#include <vector>

static void __randlib_fail(const std::string& message)
{
    debug::cerr() << message << debug::endl;
}
template <typename T>
#ifdef __GNUC__
__attribute__((const))
#endif
static inline T
__randlib_min(const T& a, const T& b)
{
    return a < b ? a : b;
}

template <typename T>
#ifdef __GNUC__
__attribute__((const))
#endif
static inline T
__randlib_max(const T& a, const T& b)
{
    return a > b ? a : b;
}
template <typename T>
#ifdef __GNUC__
__attribute__((const))
#endif
static inline T
__randlib_crop(T value, T a, T b)
{
    return __randlib_min(__randlib_max(value, a), --b);
}

#ifdef __GNUC__
__attribute__((const))
#endif
static inline double
__randlib_crop(double value, double a, double b)
{
    value = __randlib_min(__randlib_max(value, a), b);
    if (value >= b)
        value = std::nexttoward(b, a);
    return value;
}

const long long __randlib_LONGLONG_MAX = 9223372036854775807LL;

/*
 * Use random_t instances to generate random values. It is preferred
 * way to use randoms instead of rand() function or self-written
 * randoms.
 *
 * Testlib defines global variable "rnd" of random_t class.
 * Use registerGen(argc, argv, 1) to setup random_t seed be command
 * line (to use latest random generator version).
 *
 * Random generates uniformly distributed values if another strategy is
 * not specified explicitly.
 */
class random_t {
private:
    unsigned long long seed; // 生成随机种子的种子值
    static const unsigned long long multiplier; // 更新种子的乘数
    static const unsigned long long addend; // 更新种子的增量
    static const unsigned long long mask; // 用于截断种子的掩码
    static const int lim; // wnext最大值生成个数上限制

    /**
     * @brief 生成指定数量的随机位
     * @param bits 位数
     */
    long long nextBits(int bits)
    {
        if (bits <= 48) {
            seed = (seed * multiplier + addend) & mask;
            return (long long)(seed >> (48 - bits));
        } else {
            if (bits > 63)
                __randlib_fail("random_t::nextBits(int bits): n must be less than 64");

            int lowerBitCount = (random_t::version == 0 ? 31 : 32);

            long long left = (nextBits(31) << 32);
            long long right = nextBits(lowerBitCount);

            return left ^ right;
        }
    }

public:
    static int version;

    /* New random_t with fixed seed. */
    random_t()
        : seed(3905348978240129619LL)
    {
    }

    /* Sets seed by given value. */
    void setSeed(long long _seed)
    {
        seed = (unsigned long long)_seed;
        seed = (seed ^ multiplier) & mask;
    }

    /* Random value in range [0, n-1]. */
    int next(int n)
    {
        if (n <= 0)
            __randlib_fail("random_t::next(int n): n must be positive");

        if ((n & -n) == n) // n is a power of 2
            return (int)((n * (long long)nextBits(31)) >> 31);

        const long long limit = INT_MAX / n * n;

        long long bits;
        do {
            bits = nextBits(31);
        } while (bits >= limit);

        return int(bits % n);
    }

    /* Random value in range [0, n-1]. */
    unsigned int next(unsigned int n)
    {
        if (n >= INT_MAX)
            __randlib_fail("random_t::next(unsigned int n): n must be less INT_MAX");
        return (unsigned int)next(int(n));
    }

    /* Random value in range [0, n-1]. */
    long long next(long long n)
    {
        if (n <= 0)
            __randlib_fail("random_t::next(long long n): n must be positive");

        const long long limit = __randlib_LONGLONG_MAX / n * n;

        long long bits;
        do {
            bits = nextBits(63);
        } while (bits >= limit);

        return bits % n;
    }

    /* Random value in range [0, n-1]. */
    unsigned long long next(unsigned long long n)
    {
        if (n >= (unsigned long long)(__randlib_LONGLONG_MAX))
            __randlib_fail("random_t::next(unsigned long long n): n must be less LONGLONG_MAX");
        return (unsigned long long)next((long long)(n));
    }

    /* Random value in range [0, n-1]. */
    long next(long n)
    {
        return (long)next((long long)(n));
    }

    /* Random value in range [0, n-1]. */
    unsigned long next(unsigned long n)
    {
        if (n >= (unsigned long)(LONG_MAX))
            __randlib_fail("random_t::next(unsigned long n): n must be less LONG_MAX");
        return (unsigned long)next((unsigned long long)(n));
    }

    /* Returns random value in range [from,to]. */
    int next(int from, int to)
    {
        return int(next((long long)to - from + 1) + from);
    }

    /* Returns random value in range [from,to]. */
    unsigned int next(unsigned int from, unsigned int to)
    {
        return (unsigned int)(next((long long)to - from + 1) + from);
    }

    /* Returns random value in range [from,to]. */
    long long next(long long from, long long to)
    {
        return next(to - from + 1) + from;
    }

    /* Returns random value in range [from,to]. */
    unsigned long long next(unsigned long long from, unsigned long long to)
    {
        if (from > to)
            __randlib_fail("random_t::next(unsigned long long from, unsigned long long to): from can't not exceed to");
        return next(to - from + 1) + from;
    }

    /* Returns random value in range [from,to]. */
    long next(long from, long to)
    {
        return next(to - from + 1) + from;
    }

    /* Returns random value in range [from,to]. */
    unsigned long next(unsigned long from, unsigned long to)
    {
        if (from > to)
            __randlib_fail("random_t::next(unsigned long from, unsigned long to): from can't not exceed to");
        return next(to - from + 1) + from;
    }

    /* Random double value in range [0, 1). */
    double next()
    {
        long long left = ((long long)(nextBits(26)) << 27);
        long long right = nextBits(27);
        return __randlib_crop((double)(left + right) / (double)(1LL << 53), 0.0, 1.0);
    }

    /* Random double value in range [0, n). */
    double next(double n)
    {
        if (n <= 0.0)
            __randlib_fail("random_t::next(double): n should be positive");
        return __randlib_crop(n * next(), 0.0, n);
    }

    /* Random double value in range [from, to). */
    double next(double from, double to)
    {
        if (from >= to)
            __randlib_fail("random_t::next(double from, double to): from should be strictly less than to");
        return next(to - from) + from;
    }

    /* Returns random element from container. */
    template <typename Container>
    typename Container::value_type any(const Container& c)
    {
        int size = int(c.size());
        if (size <= 0)
            __randlib_fail("random_t::any(const Container& c): c.size() must be positive");
        typename Container::const_iterator it = c.begin();
        std::advance(it, next(size));
        return *it;
    }

    /* Returns random element from iterator range. */
    template <typename Iter>
    typename Iter::value_type any(const Iter& begin, const Iter& end)
    {
        int size = static_cast<int>(std::distance(begin, end));
        if (size <= 0)
            __randlib_fail("random_t::any(const Iter& begin, const Iter& end): range must have positive length");
        Iter it = begin;
        std::advance(it, next(size));
        return *it;
    }

    /*
     * Weighted next. If type == 0 than it is usual "next()".
     *
     * If type = 1, then it returns "max(next(), next())"
     * (the number of "max" functions equals to "type").
     *
     * If type < 0, than "max" function replaces with "min".
     */
    int wnext(int n, int type)
    {
        if (n <= 0)
            __randlib_fail("random_t::wnext(int n, int type): n must be positive");

        if (abs(type) < random_t::lim) {
            int result = next(n);

            for (int i = 0; i < +type; i++)
                result = __randlib_max(result, next(n));

            for (int i = 0; i < -type; i++)
                result = __randlib_min(result, next(n));

            return result;
        } else {
            double p;

            if (type > 0)
                p = std::pow(next() + 0.0, 1.0 / (type + 1));
            else
                p = 1 - std::pow(next() + 0.0, 1.0 / (-type + 1));

            return __randlib_crop((int)(double(n) * p), 0, n);
        }
    }

    /* See wnext(int, int). It uses the same algorithms. */
    long long wnext(long long n, int type)
    {
        if (n <= 0)
            __randlib_fail("random_t::wnext(long long n, int type): n must be positive");

        if (abs(type) < random_t::lim) {
            long long result = next(n);

            for (int i = 0; i < +type; i++)
                result = __randlib_max(result, next(n));

            for (int i = 0; i < -type; i++)
                result = __randlib_min(result, next(n));

            return result;
        } else {
            double p;

            if (type > 0)
                p = std::pow(next() + 0.0, 1.0 / (type + 1));
            else
                p = 1 - std::pow(next() + 0.0, 1.0 / (-type + 1));

            return __randlib_crop((long long)(double(n) * p), 0LL, n);
        }
    }

    /* Returns value in [0, n). See wnext(int, int). It uses the same algorithms. */
    double wnext(double n, int type)
    {
        if (n <= 0)
            __randlib_fail("random_t::wnext(double n, int type): n must be positive");

        if (abs(type) < random_t::lim) {
            double result = next();

            for (int i = 0; i < +type; i++)
                result = __randlib_max(result, next());

            for (int i = 0; i < -type; i++)
                result = __randlib_min(result, next());

            return n * result;
        } else {
            double p;

            if (type > 0)
                p = std::pow(next() + 0.0, 1.0 / (type + 1));
            else
                p = 1 - std::pow(next() + 0.0, 1.0 / (-type + 1));

            return __randlib_crop(n * p, 0.0, n);
        }
    }

    /* Returns value in [0, 1). See wnext(int, int). It uses the same algorithms. */
    double wnext(int type)
    {
        return wnext(1.0, type);
    }

    /* See wnext(int, int). It uses the same algorithms. */
    unsigned int wnext(unsigned int n, int type)
    {
        if (n >= INT_MAX)
            __randlib_fail("random_t::wnext(unsigned int n, int type): n must be less INT_MAX");
        return (unsigned int)wnext(int(n), type);
    }

    /* See wnext(int, int). It uses the same algorithms. */
    unsigned long long wnext(unsigned long long n, int type)
    {
        if (n >= (unsigned long long)(__randlib_LONGLONG_MAX))
            __randlib_fail("random_t::wnext(unsigned long long n, int type): n must be less LONGLONG_MAX");

        return (unsigned long long)wnext((long long)(n), type);
    }

    /* See wnext(int, int). It uses the same algorithms. */
    long wnext(long n, int type)
    {
        return (long)wnext((long long)(n), type);
    }

    /* See wnext(int, int). It uses the same algorithms. */
    unsigned long wnext(unsigned long n, int type)
    {
        if (n >= (unsigned long)(LONG_MAX))
            __randlib_fail("random_t::wnext(unsigned long n, int type): n must be less LONG_MAX");

        return (unsigned long)wnext((unsigned long long)(n), type);
    }

    /* Returns weighted random value in range [from, to]. */
    int wnext(int from, int to, int type)
    {
        if (from > to)
            __randlib_fail("random_t::wnext(int from, int to, int type): from can't not exceed to");
        return wnext(to - from + 1, type) + from;
    }

    /* Returns weighted random value in range [from, to]. */
    int wnext(unsigned int from, unsigned int to, int type)
    {
        if (from > to)
            __randlib_fail("random_t::wnext(unsigned int from, unsigned int to, int type): from can't not exceed to");
        return int(wnext(to - from + 1, type) + from);
    }

    /* Returns weighted random value in range [from, to]. */
    long long wnext(long long from, long long to, int type)
    {
        if (from > to)
            __randlib_fail("random_t::wnext(long long from, long long to, int type): from can't not exceed to");
        return wnext(to - from + 1, type) + from;
    }

    /* Returns weighted random value in range [from, to]. */
    unsigned long long wnext(unsigned long long from, unsigned long long to, int type)
    {
        if (from > to)
            __randlib_fail(
                "random_t::wnext(unsigned long long from, unsigned long long to, int type): from can't not exceed to");
        return wnext(to - from + 1, type) + from;
    }

    /* Returns weighted random value in range [from, to]. */
    long wnext(long from, long to, int type)
    {
        if (from > to)
            __randlib_fail("random_t::wnext(long from, long to, int type): from can't not exceed to");
        return wnext(to - from + 1, type) + from;
    }

    /* Returns weighted random value in range [from, to]. */
    unsigned long wnext(unsigned long from, unsigned long to, int type)
    {
        if (from > to)
            __randlib_fail("random_t::wnext(unsigned long from, unsigned long to, int type): from can't not exceed to");
        return wnext(to - from + 1, type) + from;
    }

    /* Returns weighted random double value in range [from, to). */
    double wnext(double from, double to, int type)
    {
        if (from >= to)
            __randlib_fail("random_t::wnext(double from, double to, int type): from should be strictly less than to");
        return wnext(to - from, type) + from;
    }

    /* Returns weighted random element from container. */
    template <typename Container>
    typename Container::value_type wany(const Container& c, int type)
    {
        int size = int(c.size());
        if (size <= 0)
            __randlib_fail("random_t::wany(const Container& c, int type): c.size() must be positive");
        typename Container::const_iterator it = c.begin();
        std::advance(it, wnext(size, type));
        return *it;
    }

    /* Returns weighted random element from iterator range. */
    template <typename Iter>
    typename Iter::value_type wany(const Iter& begin, const Iter& end, int type)
    {
        int size = static_cast<int>(std::distance(begin, end));
        if (size <= 0)
            __randlib_fail(
                "random_t::any(const Iter& begin, const Iter& end, int type): range must have positive length");
        Iter it = begin;
        std::advance(it, wnext(size, type));
        return *it;
    }

    /* Returns random permutation of the given size (values are between `first` and `first`+size-1)*/
    template <typename T, typename E>
    std::vector<E> perm(T size, E first)
    {
        if (size < 0)
            __randlib_fail("random_t::perm(T size, E first = 0): size must non-negative");
        else if (size == 0)
            return std::vector<E>();
        std::vector<E> p(size);
        E current = first;
        for (T i = 0; i < size; i++)
            p[i] = current++;
        if (size > 1)
            for (T i = 1; i < size; i++)
                std::swap(p[i], p[next(i + 1)]);
        return p;
    }

    /* Returns random permutation of the given size (values are between 0 and size-1)*/
    template <typename T>
    std::vector<T> perm(T size)
    {
        return perm(size, T(0));
    }

    /* Returns `size` unordered (unsorted) distinct numbers between `from` and `to`. */
    template <typename T>
    std::vector<T> distinct(int size, T from, T to)
    {
        std::vector<T> result;
        if (size == 0)
            return result;

        if (from > to)
            __randlib_fail("random_t::distinct expected from <= to");

        if (size < 0)
            __randlib_fail("random_t::distinct expected size >= 0");

        uint64_t n = to - from + 1;
        if (uint64_t(size) > n)
            __randlib_fail("random_t::distinct expected size <= to - from + 1");

        double expected = 0.0;
        for (int i = 1; i <= size; i++)
            expected += double(n) / double(n - i + 1);

        if (expected < double(n)) {
            std::set<T> vals;
            while (int(vals.size()) < size) {
                T x = T(next(from, to));
                if (vals.insert(x).second)
                    result.push_back(x);
            }
        } else {
            if (n > 1000000000)
                __randlib_fail("random_t::distinct here expected to - from + 1 <= 1000000000");
            std::vector<T> p(perm(int(n), from));
            result.insert(result.end(), p.begin(), p.begin() + size);
        }

        return result;
    }

    /* Returns `size` unordered (unsorted) distinct numbers between `0` and `upper`-1. */
    template <typename T>
    std::vector<T> distinct(int size, T upper)
    {
        if (size < 0)
            __randlib_fail("random_t::distinct expected size >= 0");
        if (size == 0)
            return std::vector<T>();

        if (upper <= 0)
            __randlib_fail("random_t::distinct expected upper > 0");
        if (size > upper)
            __randlib_fail("random_t::distinct expected size <= upper");

        return distinct(size, T(0), upper - 1);
    }

    /* Returns random (unsorted) partition which is a representation of sum as a sum of integers not less than min_part. */
    template <typename T>
    std::vector<T> partition(int size, T sum, T min_part)
    {
        if (size < 0)
            __randlib_fail("random_t::partition: size < 0");
        if (size == 0 && sum != 0)
            __randlib_fail("random_t::partition: size == 0 && sum != 0");
        if (min_part * size > sum)
            __randlib_fail("random_t::partition: min_part * size > sum");
        if (size == 0 && sum == 0)
            return std::vector<T>();

        T sum_ = sum;
        sum -= min_part * size;

        std::vector<T> septums(size);
        std::vector<T> d = distinct(size - 1, T(1), T(sum + size - 1));
        for (int i = 0; i + 1 < size; i++)
            septums[i + 1] = d[i];
        sort(septums.begin(), septums.end());

        std::vector<T> result(size);
        for (int i = 0; i + 1 < size; i++)
            result[i] = septums[i + 1] - septums[i] - 1;
        result[size - 1] = sum + size - 1 - septums.back();

        for (std::size_t i = 0; i < result.size(); i++)
            result[i] += min_part;

        T result_sum = 0;
        for (std::size_t i = 0; i < result.size(); i++)
            result_sum += result[i];
        if (result_sum != sum_)
            __randlib_fail("random_t::partition: partition sum is expected to be the given sum");

        if (*std::min_element(result.begin(), result.end()) < min_part)
            __randlib_fail("random_t::partition: partition min is expected to be no less than the given min_part");

        if (int(result.size()) != size || result.size() != (size_t)size)
            __randlib_fail("random_t::partition: partition size is expected to be equal to the given size");

        return result;
    }

    /* Returns random (unsorted) partition which is a representation of sum as a sum of positive integers. */
    template <typename T>
    std::vector<T> partition(int size, T sum)
    {
        return partition(size, sum, T(1));
    }
};

const int random_t::lim = 25;
const unsigned long long random_t::multiplier = 0x5DEECE66DLL; // 25214903917
const unsigned long long random_t::addend = 0xBLL;
const unsigned long long random_t::mask = (1LL << 48) - 1;
int random_t::version = -1;