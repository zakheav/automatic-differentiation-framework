#include "../include/ComputeGraph.h"
#include "../include/OperatorNode.h"
#include <iostream>
using namespace std;
void ComputeGraph::forward_propagation (vector<Tensor*> &result_list) {
    vector<Node*> topo_result;
    topological_sort (m_adj_table, topo_result);
    for (int i = 0; i < topo_result.size (); ++i) {
        // cout << topo_result[i] -> get_name () << endl;
        ((OperatorNode*) topo_result[i]) -> op ();
    }
    vector<Node*> endnode_list;
    get_endnode (endnode_list);
    for (int i = 0; i < endnode_list.size (); ++i) {
        result_list.push_back (((OperatorNode*) endnode_list[i]) -> m_output);
    }
}
void ComputeGraph::back_propagation () {
    vector<Node*> topo_result;
    topological_sort (m_reverse_table, topo_result);
    for (int i = 0; i < topo_result.size (); ++i) {
        // cout << topo_result[i] -> get_name () << endl;
        ((OperatorNode*) topo_result[i]) -> grad_op ();
    }
    // 更新权值
    for (int i = 0; i < topo_result.size (); ++i) {
        ((OperatorNode*) topo_result[i]) -> update ();
    }
}
void ComputeGraph::release_tensor () {
    unordered_map<string, Node*>::iterator node_map_it = m_node_map.begin ();
    while (node_map_it != m_node_map.end ()) {
        // 释放sum_grad
        if (((OperatorNode*) (node_map_it -> second)) -> m_sum_grad != 0) {
            delete ((OperatorNode*) (node_map_it -> second)) -> m_sum_grad;
            ((OperatorNode*) (node_map_it -> second)) -> m_sum_grad = 0;
            // cout << "release grad " << node_map_it -> first << endl;
        }
        if (node_map_it -> second -> m_name[0] != "Parameter" && node_map_it -> second -> m_name[0] != "Input") {// 释放非参数节点的output
            delete ((OperatorNode*) (node_map_it -> second)) -> m_output;
            ((OperatorNode*) (node_map_it -> second)) -> m_output = 0;
            // cout << "release output " << node_map_it -> first << endl;
        }
        ++node_map_it;
    }
}
ComputeGraph::~ComputeGraph () {
    cout << "compute graph free" << endl;
}
