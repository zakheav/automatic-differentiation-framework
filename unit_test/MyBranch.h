#ifndef MYBRANCH_H_
#define MYBRANCH_H_
#include "../include/BranchNode.h"
class MyBranch: public BranchNode {
    public:
        Node* m_parent_compute_node;
        MyBranch (std::string type, std::string id);
        Node* choose_node (int idx, Graph* compute_graph);
};
#endif
