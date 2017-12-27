#include "../include/ComputeGraph.h"
#include "../include/OperatorNode.h"
#include <iostream>
using namespace std;
void ComputeGraph::forward_propagation (vector<Node*> &result_list) {
    vector<Node*> topo_result;
    topological_sort (m_adj_table, topo_result);
    for (int i = 0; i < topo_result.size (); ++i) {
        ((OperatorNode*) topo_result[i]) -> op ();
    }
    get_endnode (result_list);
}
void ComputeGraph::back_propagation () {
    vector<Node*> topo_result;
    topological_sort (m_reverse_table, topo_result);
    for (int i = 0; i < topo_result.size (); ++i) {
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
        ((OperatorNode*) (node_map_it -> second)) -> release_tensor ();
        ++node_map_it;
    }
}
ComputeGraph::~ComputeGraph () {
    cout << "compute graph free" << endl;
}
