#ifndef BRANCHNODE_H_
#define BRANCHNODE_H_
#include "Node.h"
#include "Graph.h"
class BranchNode: public Node {
    public:
        // 当BrancnNode作为循环起始的一部分时，有时需要BranchNode所在的LoopNode提供初始化的计算节点名字，从到实现循环级联
        std::string m_dep_op_node_name;
        BranchNode (std::string type, std::string id);
        virtual Node* choose_node (int idx, Graph* compute_graph);
};
#endif
