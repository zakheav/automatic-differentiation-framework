#ifndef BRANCHNODE_H_
#define BRANCHNODE_H_
#include "Node.h"
#include "Graph.h"
class BranchNode: public Node {
    public:
        BranchNode (std::string type, std::string id);
        virtual Node* choose_node (int idx, Graph* compute_graph);
};
#endif
