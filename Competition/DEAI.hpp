#ifndef M_DEAI_HPP
#define M_DEAI_HPP

#include "IO.hpp"
namespace deai {
class RNN; // 1986 递归神经网络，是一种具有递归结构的神经网络，能够处理序列数据
class QLearning; // 1989 Q学习是一种基于值的强化学习算法，通过学习价值函数来实现最优策略
class SVM; // 1995 一种有效的分类算法，能够在高维空间中找到一个最优的超平面，从而实现分类
class LSTM; // 1997 长短期记忆网络，是一种特殊的RNN，能够解决长期依赖问题
class CNN; // LeNet-5(1998) AlexNet(2012) VGGNet(2014) GoogleNet(2015) ResNet(2015) DenseNet(2016) MobileNet(2017) EfficientNet(2019)
class RandomForest; // 2001 随机森林是一种集成学习方法，通过训练多个决策树来提高分类性能
class GAN; // 2014 生成对抗网络，是一种生成模型，通过博弈的方式训练生成器和判别器
class DDPG; // 2015 深度确定性策略梯度，是一种基于值的强化学习算法，通过学习Q值函数来实现最优策略
class PPOMethod; // 2017 PPO是一种策略梯度方法，通过剪切梯度来提高训练的稳定性
class Transformer; // 2017 注意力机制，是一种用于提取序列信息的机制，Transformer是一种基于注意力机制的模型
class BERT; // 2018 双向编码器表示转换，是一种预训练模型，能够学习文本的双向表示
} // namespace deai
#endif // M_DEAI_HPP