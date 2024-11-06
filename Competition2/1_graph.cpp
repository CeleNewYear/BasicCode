#include "Competition/IO.hpp"
#include <cstring>
#include <vector>

/**
 * @brief 有向图
 * @note 该图是一个邻接表的实现
 * @note 可以快速添加边，但是删除边和查询边的效率较低
 */
namespace degraph {
struct graph_t {
    std::vector<int> info; // info[i]记录i节点最后一条边在to数组中的位置
    std::vector<int> next; // 链表中下一条边在to数组中的位置
    std::vector<int> to; // to[i]表示编号为i的边指向的节点

    /**
     * @brief Construct a new graph_t object
     * @param n 节点数量
     * @param m 边数量
     */
    graph_t(int n = 0, int m = 0)
    {
        info.resize(n);
        memset(&info[0], -1, sizeof(int) * n);
        next.reserve(m);
        to.reserve(m);
    }

    int edge_size() // 返回边的数量
    {
        return to.size();
    }

    int vertex_size() // 返回节点的数量(最大节点编号+1)
    {
        return info.size();
    }
    void expand(int i) // 确保info数组的大小至少为i+1(扩展图的节点数量)
    {
        if (info.size() <= i) {
            info.resize(i + 1);
        }
    }
    void add_edge(int i, int j) // 添加从i到j的边
    {
        expand(i), expand(j);
        to.push_back(j);
        next.push_back(info[i]);
        info[i] = to.size() - 1;
    }
    void delete_edge(int i, int j) // 删除从i到j的边(最后添加的边)
    {
        int last = -1;
        for (int k = info[i]; k >= 0; k = next[k]) {
            if (to[k] == j) {
                if (last == -1) {
                    info[i] = next[k];
                } else {
                    next[last] = next[k];
                }
                return;
            }
            last = k;
        }
    }
    void clear()
    {
        info.clear();
        next.resize(0);
        to.resize(0);
    }
    void print()
    {
        debug::cerr() << "Graph Info:\n";
        for (int i = 0; i < info.size(); ++i) {
            for (int j = info[i]; j >= 0; j = next[j]) {
                debug::cerr() << i << " -> " << to[j] << "\n";
            }
        }
    }
};
} // namespace degraph

int main()
{
    degraph::graph_t g(5, 5);
    g.add_edge(0, 1);
    g.add_edge(0, 2);
    g.add_edge(2, 3);
    g.add_edge(0, 3);
    g.add_edge(0, 3);
    g.add_edge(0, 4);
    g.delete_edge(0, 3);
    g.delete_edge(2, 3);
    g.print();
    return 0;
}