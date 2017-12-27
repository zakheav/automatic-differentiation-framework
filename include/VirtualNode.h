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
        std::vector<Node*> m_op_node_list;
        int m_share_parameter;
        VirtualNode (std::string type, std::string id, int share_parameter = 0);
        void get_parents_op_nodes (int idx, Graph* compute_graph, std::vector<Node*> &node_list);
        Node* get_op_node (int idx);
        ~VirtualNode ();
};
#endif
