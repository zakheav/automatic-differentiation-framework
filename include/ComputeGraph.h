#ifndef COMPUTEGRAPH_H_
#define COMPUTEGRAPH_H_
#include "Graph.h"
#include "Node.h"
#include "Optimizer.h"
#include <vector>
class ComputeGraph: public Graph {
    public:
        Optimizer* m_optimizer;
        void forward_propagation (std::vector<Node*> &result_list);
        void back_propagation ();
        void release_tensor ();
        ~ComputeGraph ();// 并不释放其中包含的计算节点的内存空间，用于支持动态计算图
};
#endif
