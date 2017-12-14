#ifndef VIRTUALNODE_H_
#define VIRTUALNODE_H_
#include "Node.h"
#include "Tensor.h"
#include "OperatorNode.h"
#include "Graph.h"
#include <string>
#include <vector>
class VirtualNode: public Node {
    public:
        Tensor* m_data;
        std::vector<Tensor*> m_input_data;
        VirtualNode (std::string type, std::string id);
        void get_parents_op_nodes (int idx, Graph* compute_graph, std::vector<Node*> &node_list);
        Node* get_op_node (int idx);
        ~VirtualNode ();
};
#endif
