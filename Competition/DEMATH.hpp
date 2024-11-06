#include "IO.hpp"
#include <cmath>
#include <cstring>
#include <functional>
#include <vector>

/**
 * @brief 数学库
 */

namespace demath {
/**
 * @brief 贝祖定理求最大公约数
 * @param a 整数a
 * @param b 整数b
 * @return a和b的最大公约数
 *
 * @note 贝祖定理：gcd(a, b) = gcd(b, a % b)
 * @note 时间复杂度O(log(min(a, b)))
 */
int gcd(int a, int b)
{
    return b == 0 ? a : gcd(b, a % b);
}

/**
 * @brief 求x,y使得ax + by = gcd(a, b)
 */
int extend_gcd(int a, int b, int& x, int& y)
{
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    } else {
        int r = extend_gcd(b, a % b, y, x);
        y -= a / b * x;
        return r;
    }
}

/**
 * @brief 一元函数simpson法求积分
 * @param f 函数
 * @param a 下界， b 上界
 * @param n 分割数，n越大越精确
 * @return 积分结果
 */
template <typename T>
double simpson(const T& f, double a, double b, int n)
{
    const double h = (b - a) / n;
    double ans = f(a) + f(b);
    for (int i = 1; i < n; i += 2) {
        ans += 4 * f(a + i * h);
    }
    for (int i = 2; i < n; i += 2) {
        ans += 2 * f(a + i * h);
    }
    return ans * h / 3;
}

/**
 * @brief 一元函数romberg法求积分
 * @param f 函数
 * @param a 下界， b 上界
 * @param eps 精度
 */
template <typename T>
double romberg(const T& f, double a, double b, double eps = 1e-6)
{
    std::vector<double> t;
    double h = b - a, last, curr;
    int k = 1, i = 1;
    t.push_back(h * (f(a) + f(b)) / 2);
    do {
        last = t.back();
        curr = 0;
        double x = a + h / 2;
        for (int j = 0; j < k; ++j) {
            curr += f(x);
            x += h;
        }
        curr = (t[0] + h * curr) / 2;
        double k1 = 4.0 / 3.0;
        double k2 = 1.0 / 3.0;
        for (int j = 0; j < i; j++) {
            double temp = k1 * curr - k2 * t[j];
            t[j] = curr;
            curr = temp;
            k2 /= 4 * k1 - k2; // 防止溢出
            k1 = k2 + 1;
        }
        h /= 2;
        k *= 2;
        i++;
        t.push_back(curr);
    } while (std::abs(curr - last) > eps);
    return curr;
}

/**
 * @brief 一元函数求梯度
 * @param f 函数
 * @param x 求导点
 * @param h 步长
 * @return 梯度
 */
template <typename T>
double gradient(const T& f, double x, double h = 1e-6)
{
    return (f(x + h) - f(x - h)) / (2 * h);
}
} // namespace demath