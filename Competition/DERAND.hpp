#ifndef MY_DERAND_HPP
#define MY_DERAND_HPP
/**
 * @author GuXinyang
 * @date 2024/11/5
 *
 * @brief namespace derand 提供了随机数生成相关的函数
 *
 * @brief 设置随机数种子 set_seed
 * @brief 轮盘赌选择 roulette_wheel_selection
 * @brief 随机交换两个元素 swap_two_element_randomly
 * @brief levy flight步长生成 levy_flight
 *
 *
 * @example 使用示例
 * int main()
 * {
 *     std::vector<double> weights = { 0.1, 0.2, 0.3, 0.4 };
 *     std::vector<int> cnt(weights.size(), 0);
 *     for (int i = 0; i < 1000000; i++) {
 *         int index = derand::roulette_wheel_selection(weights);
 *         cnt[index]++;
 *     }
 *     for (int i = 0; i < int(cnt.size()); i++) {
 *         debug::cerr() << i << " " << cnt[i] << "\n";
 *     }
 *     return 0;
 * }
 */

#include "IO.hpp"
#include <cstdio>
#include <iostream>
#include <numeric>
#include <random>
namespace derand {
std::mt19937 engine;
unsigned long long ENGINE_MAX = 4294967295;

/**
 * @brief 设置随机数种子
 * @param seed 种子
 */
void set_seed(unsigned long long seed)
{
    engine.seed(seed);
}

/**
 * @brief 轮盘赌选择 按照权重随机选择一个元素
 * @param weights 权重数组
 * @return 选中的元素下标
 */
int roulette_wheel_selection(std::vector<double> weights)
{
#ifdef GXY_DEBUG
    for (auto& w : weights)
        if (w < 0) {
            debug::cerr() << "roulette_wheel_selection: weights must be non-negative\n";
            throw std::runtime_error("roulette_wheel_selection: weights must be non-negative");
        }
#endif
    double sum = 0;
    for (auto& w : weights)
        sum += w;
    double r = double(engine()) / ENGINE_MAX * sum;
    for (int i = 0; i < int(weights.size()); i++) {
        r -= weights[i];
        if (r <= 0)
            return i;
    }
    return int(weights.size()) - 1;
}

/**
 * @brief 随机交换两个元素
 * @param v 数组
 */
template <typename T>
inline void swap_two_element_randomly(std::vector<T>& v)
{
    if (v.size() < 2) {
        return;
    }
    int i = engine() % v.size();
    int j = engine() % v.size();
    std::swap(v[i], v[j]);
}

/**
 * @brief levy flight
 * @attention 随机步长的运动模式，特点是间隔较短或偶尔较长的跳跃
 *            levy分布，是一种重尾分布，是一种概率分布，其概率密度函数在正无穷大处有一个尾部
 *            概率密度函数p(x) = 1 / (len ** (1 + beta)) 其中len是步长，beta是分布的形状参数
 *            具体使用时，新的位置 = 旧的位置 + alpha * Levy(beta)
 * @param beta 分布的形状参数
 * @param scale 步长更新系数
 */
double levy_flight(double beta, double alpha)
{
    // 生成标准正态分布的两个独立变量 u 和 v
    std::normal_distribution<double> normal(0.0, 1.0);
    double u = normal(engine);
    double v = normal(engine);

    // 使用Mantegna的方法生成Levy分布
    double sigma_u = pow((tgamma(1 + beta) * sin(M_PI * beta / 2)) / (tgamma((1 + beta) / 2) * beta * pow(2, (beta - 1) / 2)), 1.0 / beta);
    double levy_step = alpha * (u * sigma_u) / pow(fabs(v), 1.0 / beta);

    return levy_step;
}
} // namespace derand;

#endif // MY_DERAND_HPP