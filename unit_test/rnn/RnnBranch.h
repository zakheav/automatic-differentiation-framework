#ifndef RNNBRANCH_H_
#define RNNBRANCH_H_
#include "../../include/BranchNode.h"
class RnnBranch: public BranchNode {
    public:
        RnnBranch (std::string type, std::string id);
        virtual Node* choose_node (int idx, Graph* compute_graph);
};
#endif
