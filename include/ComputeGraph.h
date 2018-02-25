#ifndef COMPUTEGRAPH_H_
#define COMPUTEGRAPH_H_
#include "Graph.h"
#include "Node.h"
#include "Optimizer.h"
#include <vector>
class ComputeGraph: public Graph {
    protected:
        int m_need_release_tensor_flag;
        void release_tensor ();
    public:
        ComputeGraph ();
        Optimizer* m_optimizer;
        void forward_propagation (std::vector<Node*> &result_list);
        void back_propagation ();
        ~ComputeGraph ();// 并不释放其中包含的计算节点的内存空间，用于支持动态计算图
};
#endif
