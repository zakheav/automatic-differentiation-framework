#include "../include/BranchNode.h"
#include "../include/op_node/Parameter.h"
#include "../include/Tensor.h"
#include <iostream>
#include <vector>
using namespace std;
BranchNode::BranchNode (string type, string id): Node (type, id) {
}
Node* BranchNode::choose_node (int idx, Graph* compute_graph) {
    return 0;
}
